#include "../include/Server.hpp"

Server::Server(int port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1)
{
    char hostname[HOST_NAME_MAX + 1];
    int result = gethostname(hostname, sizeof(hostname));
    if (result == 0)
        serverName = hostname;
    else
        serverName = "Server";
}

Server::~Server()
{
    for(size_t i = 0; i < _pollfds.size(); i++)
        close(_pollfds[i].fd);
    _clients.clear();
    _pollfds.clear();
}

void Server::start()
{
    createSocket();
    setupSocket();
    runPollLoop();
}

Client *Server::getClient(int clientFd)
{
    std::map<int, Client>::iterator it = _clients.find(clientFd);
    if (it == _clients.end())
        return NULL;
    return &(it->second);
}

void Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set server socket to non-blocking mode"); // throwing an exception here will terminate the program, which is appropriate since we can't proceed without a non-blocking server socket.
    std::cout << IRC_GREEN << "Server socket created: " << _serverSocket << std::endl << IRC_RESET;
}

void Server::setupSocket()
{
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt failed");
    
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    if (listen(_serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");

    std::cout << IRC_GREEN << "Server is listening on port " << _port << std::endl << IRC_RESET;

    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;
    _pollfds.push_back(serverPollFd);
}

void Server::removeClient(int clientFd)
{
    close(clientFd);// free the kernel resource
    _clients.erase(clientFd);// remove the client from the map
    for (size_t i = 0; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].fd == clientFd)
        {
            _pollfds.erase(_pollfds.begin() + i);
            break;
        }
    }
    std::cout << IRC_RED << "Client disconnected fd= " << clientFd << std::endl << IRC_RESET;
}
/* 
** here we should add the prefix with the line to specify who send the message 
** to any user using client.getPrefix()
** No I think we don't because we already have every client has it's fd so we don't need the prefix
** BUt in a normal irc server when the clients sends a message to the client the server parse it and 
** add that prefix at the first of the message or line : put in you head: it looks like that
**         >> :nick!user@host  ---- line ----<<
*/

/*
------------------------------- i'm working here----------------------------------------- 
*/

void Server::listAllUsers(Client &sender)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client target = it->second;
        this->sendMessageToClient(sender.getFd(), whoMessage(*this,target));
    }
}

void Server::despatchMessage(Client &client, const Message &msg)
{
    if (msg.getCommand() == "PRIVMSG")
          privmsg(client, msg);
    else if (msg.getCommand() == "NOTICE")
        notice(client, msg);
    else if (msg.getCommand() == "NICK")
        nickHandler(client, msg);
    else if (msg.getCommand() == "KICK")
        compare_nickname_and_kickClient(msg, client);
    else if (msg.getCommand() == "USER")
        userHandler(client, msg);
    else if (msg.getCommand() == "NAMES")
        names(client, msg);
    else if (msg.getCommand() == "PASS")
        passHandler(client, msg);
    else if (msg.getCommand() == "HOST")
        client.setHostname(msg.getParameter(0));
    else if (msg.getCommand() == "QUIT")
        client.setExited(true);
    else if (msg.getCommand() == "PING")
        this->sendMessageToClient(client.getFd(), pong(*this, msg));
    else if (msg.getCommand() == "WHO")
        who(client, msg);
    else if (msg.getCommand() == "PART")
        clientLeaveChannel(msg, client);
    else if (msg.getCommand() == "JOIN")
        check_Channels_and_addMember_to_Channel(msg, client);
    else if (msg.getCommand() == "INVITE")
        compare_nickname_and_inviteClient(msg, client);
    else if (msg.getCommand() == "TOPIC")
        showTopic(msg, client);
    else if (msg.getCommand() == "MODE")
        setMode(msg, client);
    else if (msg.getCommand() == "LIST")
        listChanels(client, msg);
    else if (msg.getCommand() == "CAP")
        return;
    // else if (msg.getCommand() == "L7AJ")
    //     bot(msg, client);
    else
    {
        this->sendMessageToClient(client.getFd(), unknownCommand(*this));
        return;
    }
}
/*
**
** getCLients_by_nickname**
**
** arg& nicknames : is a string of nicknames of separated by comma
** 
*/
std::vector<Client *> Server::getClientsByNickname(const std::string &nicknames)
{
    std::vector<Client *> clients;
    size_t start = 0;
    size_t end = nicknames.find(',');

    while (end != std::string::npos)
    {
        std::string nickname = nicknames.substr(start, end - start);
        Client *client = getClientByNickname(nickname);
        if (client)
            clients.push_back(client);
        start = end + 1;
        end = nicknames.find(',', start);
    }
    std::string nickname = nicknames.substr(start);
    Client *client = getClientByNickname(nickname);
    if (client)
        clients.push_back(client);
    return clients;
}

Client *Server::getClientByNickname(const std::string &nickname)
{
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (compareNick(it->second.getNickname(), nickname))
            return &it->second;
    }
    return NULL;
}

void Server::tryRegister(Client &client)
{
    if (client.isRegistered())
        return;

    if (client.hasPassAccepted()
        && !client.getNickname().empty()
        && !client.getUsername().empty())
    {
        client.setRegistered(true);
        sendMessageToClient(client.getFd(), welcomeMessage(*this, client));
        sendMessageToClient(client.getFd(), noMotd(*this, client));
        sendMessageToClient(client.getFd(), serverNotice(*this));
        std::cout << IRC_GREEN << "Client registered!" << std::endl << IRC_RESET;
    }
}

void Server::removeExitedClientInChannels(const Client &client)
{
    std::map<std::string, Channel>::iterator mapIter = _channels.begin();

    for (; mapIter != _channels.end();)
    {
        if (mapIter->second.isMember(client.getFd()))
            mapIter->second.leaveChannel(mapIter->first, client.getFd());
        if (mapIter->second.isOperator(client.getFd()))
            mapIter->second.removeOperator(client.getFd());
        if (mapIter->second.getEmpty())
            _channels.erase(mapIter++);
        else
            ++mapIter;
    }
}

void Server::processClientBuffer(Client &client)
{
    size_t pos;
 
    while ((pos = client.getBuffer().find("\r\n")) != std::string::npos)
    {
        Parser parser;
        Message mesg = parser.parse(client.getBuffer().substr(0, pos));
        despatchMessage(client, mesg);
        for (std::map<std::string, Channel>::iterator itChan = _channels.begin(); itChan != _channels.end();)
        {
            if (itChan->second.getEmpty())
                _channels.erase(itChan++);
            else
                ++itChan;
        }
        if (client.getExited())
        {
            removeExitedClientInChannels(client);
            removeClient(client.getFd());
            break;
        }
        client.getBuffer().erase(0, pos + 2);
    }
}
bool Server::receiveClientMessage(int clientFd)
{
    char buffer[1024];
    int bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == 0)
    {
        std::map<int, Client>::iterator it = _clients.find(clientFd);
        if (it != _clients.end())
            removeExitedClientInChannels(it->second);
        removeClient(clientFd);
        return false; 
    }
    if (bytesReceived == -1)
    {
        std::cout << "recv() failed for client fd= " << clientFd << std::endl;
        removeClient(clientFd);
        return false;
    }
    buffer[bytesReceived + 1] = '\0';

    std::map<int, Client>::iterator it = _clients.find(clientFd);

    if (it == _clients.end())
    {
        std::cerr << "Error: Client with fd " << clientFd << " not found in _clients map." << std::endl;
        return true;
    }
    it->second.getBuffer().append(buffer, bytesReceived);

    Client &client = it->second;
    processClientBuffer(client);
    return true;
}

bool Server::running = true;
void Server::signalHandler(int signum)
{
    (void)signum;
    Server::running = false;
}

void Server::runPollLoop()
{
    std::cout << IRC_GREEN << "Server is running..." << std::endl << IRC_RESET;
    
    while (Server::running)
    {
        int ret = poll(_pollfds.data(), _pollfds.size(), -1);
        if (ret == -1)
        {   
            if (errno == EINTR)
                continue;
            throw std::runtime_error("poll failed");
        }
        for (size_t i = 0; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN)
            {
                if (_pollfds[i].fd == _serverSocket)
                    acceptClient();
                else
                {
                    if (!receiveClientMessage(_pollfds[i].fd))
                        i--; // Decrement i to recheck the current index after removing a client
                }
            }
        }
    }
}

void Server::acceptClient()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_serverSocket, (sockaddr *)&clientAddr, &clientLen);

    if (clientFd == -1)
        throw std::runtime_error("accept failed");

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(clientFd);
        std::cerr << IRC_RED << "Failed to set client socket to non-blocking mode" << std::endl
                  << IRC_RESET;
        return;
    }

    _clients.insert(std::make_pair(clientFd, Client(clientFd)));
    
    pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    _pollfds.push_back(clientPollFd);

    std::cout << "Client connected fd= " << clientFd << std::endl;
    sendMessageToClient(clientFd, entringMessage(*this));
}

void Server::sendMessageToClient(int fd, const std::string &msg)
{
    // std::cout << "SEND[" << fd << "] ";

    // for (size_t i = 0; i < msg.size(); i++)
    // {
    //     if (msg[i] == '\r')
    //         std::cout << "\\r";
    //     else if (msg[i] == '\n')
    //         std::cout << "\\n";
    //     else
    //         std::cout << msg[i];
    // }

    // std::cout << std::endl;
    send(fd, msg.c_str(), msg.size(), 0);
}


void Server::broadcastToChanel(Channel &channel, const Client& sender, const std::string &msg)
{
    std::cout << "client Prefix : [" <<  sender.getPrefix() << "]" << std::endl;
    std::cout << "message : [" << msg << "]" << std::endl;
    std::vector<int> &members = channel.getMembers();
    std::vector<int>::iterator it = members.begin();

    for (; it != members.end(); it++)
    {
        if (*it != sender.getFd())
            sendMessageToClient(*it, msg);
    }
}

std::string Server::getServerName() const
{
    return serverName;
}

Channel *Server::getChanel(const std::string &chanNeame)
{
    std::map<std::string, Channel>::iterator it = _channels.find(chanNeame);

    if (it != _channels.end())
        return &it->second;
    return NULL;
}

std::map<std::string, Channel> *Server::getChannels()
{
    return &_channels;
}

std::string Server::getChanelUsers(const std::string &channelName)
{
    Channel &itchan = _channels.find(channelName)->second;
    std::vector<int> vecInt = itchan.getMembers();
    std::vector <int> ope = itchan.getOperators();
    std::map<int, Client> &clients = getClients();
    std::string names;

    for (size_t i = 0; i < vecInt.size(); i++)
    {
        std::map<int, Client>::iterator iter = clients.find(vecInt[i]);
        if (iter != clients.end())
        {
            Client &client = iter->second;
            if (itchan.isOperator(client.getFd()))
                names += "@";
            names += client.getNickname() + " ";
        }
    }
    return names;
}

std::map<int, Client> &Server::getClients()
{
    return _clients;
}

Channel* Server::getChannelbyName(const std::string &name)
{
    std::map<std::string, Channel>::iterator iter = _channels.find(name);
    if (iter != _channels.end())
        return &iter->second;
    return NULL;
}


std::vector<Channel *> Server::getChannelsByName(const std::string &channels)
{
    std::vector<Channel *> channelsV;
    size_t start = 0;

    while (start < channels.length())
    {
        size_t end = channels.find(',', start);
        std::string name;
        if (end == std::string::npos)
            name = channels.substr(start);
        else
            name = channels.substr(start, end - start);
        // Ignore empty names
        if (!name.empty())
        {
            if (name[0] == '#')
            {
                Channel *channel = getChannelbyName(name);
                if (!channel)
                {
                    std::cout << "Channel created: " << name << std::endl;
                    _channels.insert(std::make_pair(name, Channel(name)));
                    channel = getChannelbyName(name);
                }
                channelsV.push_back(channel);
            }
        }
        if (end == std::string::npos)
            break;
        start = end + 1;
    }
    return channelsV;
}