#include "../include/Server.hpp"

Server::Server(int port, const std::string &password)
    : _port(port), _password(password), _serverSocket(-1)
{
}

Server::~Server()
{
    for(size_t i = 0; i < _pollfds.size(); i++)
        close(_pollfds[i].fd);
    std::cout << "Server shutting down..." << std::endl;
}

void Server::start()
{
    createSocket();
    setupSocket();
    runPollLoop();
}

void Server::createSocket()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (_serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
    std::cout << "Server socket created: " << _serverSocket << std::endl;
}
/*
** setupSocket()
**
** Prepares the server socket to accept incoming client connections.
** This function must be called after the socket fd has been created (in the constructor),
** and before the main poll() loop starts. It performs three steps:
**
** -----------------------------------------------------------------------
** STEP 1 — setsockopt(SO_REUSEADDR) "Configure a socket option inside the kernel"
** -----------------------------------------------------------------------
** By default, after a server crashes or is killed, the OS keeps the port in a
** TIME_WAIT state for ~60 seconds, refusing any new bind() on the same port.
** SO_REUSEADDR tells the kernel to allow reuse of that port immediately.
** Without this, restarting the server during development would give:
**   "bind failed: Address already in use"
**
**   _serverSocket : the fd created by socket() in the constructor
**   SOL_SOCKET    : we're setting an option at the socket level => SOL_SOCKET → general socket, IPPROTO_TCP → TCP options, IPPROTO_IP → IP options 
**   SO_REUSEADDR  : the specific option — allow port reuse
**   &opt (= 1)    : enables the option
**
** -----------------------------------------------------------------------
** STEP 2 — bind() "Attach socket → IP + PORT"
** -----------------------------------------------------------------------
** Associates the socket with a local address and port so clients know
** where to connect. We fill a sockaddr_in struct:
**
**   sin_family      = AF_INET        → IPv4
**   sin_port        = htons(_port)   → port in network byte order (big-endian)
**   sin_addr.s_addr = INADDR_ANY     → accept connections on any network interface
**                                      (localhost, LAN, etc.)
**
** htons() is critical: x86 CPUs are little-endian, but the network protocol
** expects big-endian. Forgetting htons() causes clients to connect to a
** completely wrong port with no obvious error.
**
** -----------------------------------------------------------------------
** STEP 3 — listen()
** -----------------------------------------------------------------------
** Marks the socket as passive — it will now accept incoming connections
** instead of being used to connect outward.
**
** The second argument (128) is the backlog: the maximum number of pending
** connections the kernel will queue while our server is busy in poll().
** If the queue is full, new connection attempts are silently dropped.
** 128 is a common safe value for IRC servers at this scale.
**
** After listen() returns, the socket is ready. The actual acceptance of
** individual clients happens later in the poll() loop via accept().
*/

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
    for (size_t i = 0; i < _pollfds.size(); i++)
    {
        if (_pollfds[i].fd == clientFd)
        {
            _pollfds.erase(_pollfds.begin() + i);
            break;
        }
    }
    std::cout << "Client disconnected fd= " << clientFd << std::endl;
    printPollFds();//debug
}

void Server::receiveClientMessage(int clientFd)
{
    char buffer[1024];
    int bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived <= 0)
    {
        removeClient(clientFd);
        return;
    }

    std::map<int, Client>::iterator it = _clients.find(clientFd);
    
    if (it == _clients.end())
        return;
    it->second.getBuffer().append(buffer, bytesReceived); // Append the received message to the client's buffer

    Client &client = it->second;
    processClientBuffer(client);
}

void Server::processClientBuffer(Client &client)
{
    size_t pos;

    while ((pos = client.getBuffer().find("\r\n")) != std::string::npos)
    {
        std::string command = client.getBuffer().substr(0, pos);
        std::cout << "Complete command: [" << command << "]" << std::endl;
        client.getBuffer().erase(0, pos + 2);
    }
    std::cout << "Current buffer: [" << client.getBuffer() << "]" << std::endl;   
}

void Server::runPollLoop()
{
    std::cout << "Server is running..." << std::endl;

    while (true)
    {
        int ret = poll(_pollfds.data(), _pollfds.size(), -1);
        if (ret < 0)
            throw std::runtime_error("poll failed");
        for (size_t i = 0; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN)
            {
                if (_pollfds[i].fd == _serverSocket)
                    acceptClient();
                else
                    receiveClientMessage(_pollfds[i].fd);
            }
        }
    }
}

void Server::acceptClient()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_serverSocket, (sockaddr *)&clientAddr, &clientLen);

    if (clientFd < 0)
        throw std::runtime_error("accept failed");

    _clients.insert(std::make_pair(clientFd, Client(clientFd)));//store the client in the map with its fd as the key
    
    // Add the client socket to the pollfd vector to monitor for incoming messages
    pollfd clientPollFd;
    clientPollFd.fd = clientFd;//4
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0; // Initialize revents
    _pollfds.push_back(clientPollFd);

    std::cout << "Client connected fd= " << clientFd << std::endl;
    printPollFds();//debug
    printClients();//debug
}

void Server::printPollFds()
{
    std::cout << "\n_pollfds: [ ";
    for (size_t i = 0; i < _pollfds.size(); i++)
    {
        std::cout << _pollfds[i].fd;
        if (i != _pollfds.size() - 1)
            std::cout << ", ";
    }
    std::cout << " ]" << std::endl;
}

void Server::printClients()
{
    std::cout << "\n_clients:\n";
    for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        std::cout << "Key(fd): " << it->first << " -> Client fd: " << it->second.getFd() << std::endl;
    }
    std::cout << std::endl;
}





