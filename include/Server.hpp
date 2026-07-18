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
#include <fcntl.h>
#include "Client.hpp"
#include "Channel.hpp"


struct JoinInfo
{
    std::string channel;
    std::string Password;
};

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
    std::string Secret;
    std::string userLimit;
};


class Server
{
    private:
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
        void removeClient(int clientFd);
        //rida
        std::string getPassword(std::string command);
        std::string getNickname(std::string command);
        std::string getUsername(std::string command);
        void tryRegister(Client &client);
        JoinInfo getJoin(std::string command);
        std::string getPart(std::string command);
        void addMemberTo_Channel(JoinInfo join_info, Client &client);
        void check_Channels_and_addMember_to_Channel(JoinInfo join_info, Client &client);
        void compare_nickname_and_kickClient(std::string &channelName, std::string &nickname, Client &client);
        void compare_nickname_and_inviteClient(std::string &channelName, std::string &nickname, Client &client);
        void clientLeaveChannel(std::string channelName, Client &client);
        Kickinfo    getKickInfo(std::string command);
        Inviteinfo  getInviteInfo(std::string command);
        Topicinfo   getTopicInfo(std::string command);
        void        showTopic(Topicinfo topic_info, Client &client);
        Modeinfo    getModeInfo(std::string command);
        void setMode(Modeinfo mode_info, Client &client);

    private:
        void createSocket();
        void setupSocket();
        void runPollLoop();
        void acceptClient();
        bool receiveClientMessage(int clientFd);
        void processClientBuffer(Client &client);
};

#endif
