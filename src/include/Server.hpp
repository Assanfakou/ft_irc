#ifndef SERVER_HPP
#define SERVER_HPP

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
#include "Client.hpp"

class Server
{
    private:
        int _port;
        std::string _password;
        int _serverSocket;
        std::vector<pollfd> _pollfds;
        std::map<int, Client> _clients; // Map to store clients with their file descriptors as keys

    public:
        Server(int port, const std::string &password);
        ~Server();

        void start();
        void printPollFds();//for debugging
        void printClients();//for debugging

    private: //helpers
        void createSocket();
        void setupSocket();
        void runPollLoop();
        void acceptClient();
        void receiveClientMessage(int clientFd);
        void removeClient(int clientFd);
        void processClientBuffer(Client &client);
};

#endif
