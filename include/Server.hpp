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
#include <climits>
#include <fcntl.h>
#include <string>
#include <sstream>

#include "Client.hpp"
#include "Message.hpp"
#include "Parser.hpp"
#include "Channel.hpp"
#include "Reply.hpp"
#include "commands.hpp"

struct Kickinfo
{   
    std::string channel;
    std::string nickname;
};

struct Inviteinfo
{
    std::string channel;
    std::string nickname;
};

struct Topicinfo
{
    std::string channel;
    std::string topic;
};

struct Modeinfo
{
    std::string channel;
    std::string mode;
    std::string Operator;
};

class Server
{
    private:
        std::string serverName;
        int _port;
        std::string _password;
        int _serverSocket;
        std::vector<pollfd> _pollfds;
        std::map<int, Client> _clients; // Map to store clients with their file descriptors as keys
        //rida
        std::map<std::string, Channel> _channels;


    public:
        Server(int port, const std::string &password);
        ~Server();

        void start();
        Client *getClient(int clientFd); 
        void sendMessageToClient(int clientFd, const std::string &message);
        void broadcastToChanel(Channel &chanel, const Client &client, const std::string &msg);
        void removeClient(int clientFd);
        //rida
        const std::string &getPassword()const;
        std::string getNickname(std::string command);
        std::string getUsername(std::string command);
        void tryRegister(Client &client);
        std::string getJoin(std::string command);
        std::string getPart(std::string command);
        void addMemberTo_Channel(std::string channelName, Client &client);
        void check_Channels_and_addMember_to_Channel(std::string channelName, Client &client);
        void compare_nickname_and_kickClient(const std::string &channelName, const std::string &nickname, Client &client);
        void compare_nickname_and_inviteClient(const std::string &channelName, const std::string &nickname, Client &client);
        void clientLeaveChannel(std::string channelName, Client &client);
        Kickinfo    getKickInfo(std::string command);
        Inviteinfo  getInviteInfo(std::string command);
        Topicinfo   getTopicInfo(std::string command);
        void        showTopic(const Message &msg, Client &client);
        Modeinfo    getModeInfo(std::string command);
        void setMode(const Message &msg, Client &client);

    private:
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
        Channel *getChanel(const std::string &cnanNeame);
        std::map<std::string, Channel> *getChannels();
        void listChanels(const Client &client, const Message &msg);
        std::map<int, Client> &getClients();
        std::string getChanelUsers(const std::string &channelName);
};

#endif
