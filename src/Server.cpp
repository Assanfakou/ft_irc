#include "../include/Server.hpp"

Server::Server(int port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1)
{
}

Server::~Server()
{
    for(size_t i = 0; i < _pollfds.size(); i++)
        close(_pollfds[i].fd);
    _clients.clear();//not strictly necessary, but good practice to clear the map of clients
    _pollfds.clear();//not strictly necessary, but good practice to clear the vector of pollfds
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
    std::cout << "Server socket created: " << _serverSocket << std::endl;
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

    if (listen(_serverSocket, 128) < 0)
        throw std::runtime_error("listen failed");

    std::cout << "Server is listening on port " << _port << std::endl;

    // add the server socket to the pollfd vector to monitor for incoming connections
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;// read from this fd // I am interested in knowing when this fd becomes readable.
    serverPollFd.revents = 0; // Initialize revents
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
    std::cout << "Client disconnected fd= " << clientFd << std::endl;
}


//rida

void Server::tryRegister(Client &client)
{
    if (client.hasPassAccepted() && !client.getNickname().empty() && !client.getUsername().empty())
    {
        client.setRegistered(true);
        std::cout << "Client registered!" << std::endl;
    }
}

//rida (i only add my own code here , the function created by anass)
void Server::processClientBuffer(Client &client)
{
    size_t pos;

    while ((pos = client.getBuffer().find("\r\n")) != std::string::npos)
    {
        std::string command = client.getBuffer().substr(0, pos);
        //rida
        std::string password = getPassword(command);
        std::string nickname = getNickname(command);
        std::string username = getUsername(command);
        JoinInfo join_info = getJoin(command);
        std::string leaveChannel = getPart(command);
        Kickinfo kick_info = getKickInfo(command);
        Inviteinfo invite_info = getInviteInfo(command);
        Topicinfo topic_info = getTopicInfo(command);
        Modeinfo mode_info = getModeInfo(command);

        if (!password.empty())
        {
            if (_password == password)
            {
                client.setPassAccepted(true);
                std::cout << "Password Accepted" << std::endl;
                tryRegister(client);
            }
            else
                std::cout << "Wrong Password" << std::endl;
        }
        if (!nickname.empty())
        {
            client.setNickname(nickname);
            std::cout << "Nickname saved: " << client.getNickname() << std::endl;
            tryRegister(client);
        }
        if (!username.empty())
        {
            client.setUsername(username);
            std::cout << "Username saved: " << client.getUsername() << std::endl;
            tryRegister(client);
        }
        if (client.isRegistered())
        {
            if (!mode_info.channel.empty() && !mode_info.mode.empty())
                setMode(mode_info, client);
            if (!join_info.channel.empty())
                check_Channels_and_addMember_to_Channel(join_info, client);
            if (!leaveChannel.empty())
                clientLeaveChannel(leaveChannel, client);
            if (!kick_info.channel.empty() && !kick_info.nickname.empty())
                compare_nickname_and_kickClient(kick_info.channel, kick_info.nickname, client);
            if (!invite_info.channel.empty() && !invite_info.nickname.empty())
                compare_nickname_and_inviteClient(invite_info.channel, invite_info.nickname, client);
            if (!topic_info.channel.empty())
                showTopic(topic_info, client);
        }

        //
        client.getBuffer().erase(0, pos + 2);
    }
}


bool Server::receiveClientMessage(int clientFd)
{
    char buffer[1024];
    int bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived == 0)
    {
        removeClient(clientFd);
        return false; // The client no longer exists.
    }
    if (bytesReceived == -1)
    {
        std::cout << "recv() failed for client fd= " << clientFd << std::endl;
        removeClient(clientFd);
        return false;// The client no longer exists.
    }

    std::map<int, Client>::iterator it = _clients.find(clientFd);

    if (it == _clients.end())
    {
        std::cerr << "Error: Client with fd " << clientFd << " not found in _clients map." << std::endl;
        return true; // The client is still connected, but we couldn't find it in the map.
    }
    it->second.getBuffer().append(buffer, bytesReceived);

    Client &client = it->second;
    processClientBuffer(client);
    return true;// The client is still connected.
}
void Server::runPollLoop()
{
    std::cout << "Server is running..." << std::endl;
    
    while (true)
    {
        int ret = poll(_pollfds.data(), _pollfds.size(), -1);
        if (ret == -1)
            throw std::runtime_error("poll failed");
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
        close(clientFd); // Close the client socket if we can't set it to non-blocking mode
        std::cerr << "Failed to set client socket to non-blocking mode" << std::endl; // ignore the client if we can't set it to non-blocking mode
        return;
    }

    _clients.insert(std::make_pair(clientFd, Client(clientFd)));//store the client in the map with its fd as the key
    
    // Add the client socket to the pollfd vector to monitor for incoming messages
    pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    _pollfds.push_back(clientPollFd);

    std::cout << "Client connected fd= " << clientFd << std::endl;
}

void Server::sendMessageToClient(int clientFd, const std::string &message)
{
    if (send(clientFd, message.c_str(), message.size(), 0) == -1)
        std::cerr << "Failed to send message to client " << clientFd << std::endl;
}