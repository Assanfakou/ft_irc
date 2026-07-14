#include "../include/Server.hpp"
#include "../include/Parser.hpp"
#include "commands/commands.hpp"

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

void Server::despatchMessage(Client &client, const Message &msg)
{
    if (msg.getCommand() == "PRIVMSG")
          privmsg(*this, client, msg);
    else if (msg.getCommand() == "NICK")
    {
        client.setNickname(msg.getParameter(0));
    }
}

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

    // Handle the last nickname (or the only one if there were no commas)
    std::string nickname = nicknames.substr(start);
    Client *client = getClientByNickname(nickname);
    if (client)
        clients.push_back(client);

    return clients;
}
Client *Server::getClientByNickname(const std::string &nicknames)
{
    if (nicknames.find(',') != std::string::npos)
    {
        std::cout << nicknames << " multiple users\n";
        return NULL;
    }
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second.getNickname() == nicknames)
            return &(it->second);
    }
    return NULL;
}

void Server::processClientBuffer(Client &client)
{
    size_t pos;
    client.setUsername("Username");
    client.setHostname("linux");
 
    pos = client.getBuffer().find("\n");
    // std::cout << client.getPrefix() << "\n";
    std::cout << "length: " << pos << std::endl;
    // std::cout << "{" << client.getBuffer().substr(0, pos) << '}' << std::endl;
    Parser parser;
    Message mesg = parser.parse(client.getBuffer().substr(0, pos));
    despatchMessage(client, mesg);

    client.getBuffer().erase(0, pos + 2); // Remove
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
    buffer[bytesReceived] = '\n'; // Null-terminate the received data
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