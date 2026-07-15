#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdlib> // i added this for std::atoi 
#include <iostream>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <stdexcept>//exception
#include <netinet/in.h>// IPv4 structures and constants
#include <arpa/inet.h>// IP address conversion utilities
#include <unistd.h>
#include <cstring>
#include <map>
#include <fcntl.h>
#include "Client.hpp"
#include <climits>

class Message;

class Server
{
    private:
        std::string serverName;
        int _port;
        std::string _password;
        int _serverSocket;
        std::vector<pollfd> _pollfds;
        std::map<int, Client> _clients; // Map to store clients with their file descriptors as keys

    public:
        Server(int port, const std::string &password);
        ~Server();

        void start();
        Client *getClient(int clientFd);
        void sendMessageToClient(int clientFd, const std::string &message);
        void removeClient(int clientFd);
        
    private: //helpers
        void createSocket();
        void setupSocket();
        void runPollLoop();
        void acceptClient();
        bool receiveClientMessage(int clientFd);
        void processClientBuffer(Client &client);

        // creadted by Assan
        void despatchMessage(Client &client, const Message &message);
    public:
        Client *getClientByNickname(const std::string &nickname);
        std::vector<Client *> getClientsByNickname(const std::string &nicknames);
        void listAllUsers(Client &sender);

        std::string getServerName() const;
};

#endif
