#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "colors.h"
#include <string>

class Client
{
    private:
        int _fd;
        bool _isRegistered; // Flag to indicate if the client is registered
        // by Rida
        bool _passAccepted;
        std::string _buffer; // Buffer to store incoming data
        std::string _nickname; // Client's nickname
        std::string _username; // Client's username
        std::string _hostname; // Client's hostname
        std::string _realname; // Client's real name

    public:
        Client(int fd);

        // Getters
        int getFd() const;
        std::string &getBuffer();
        
        const std::string &getNickname() const;
        const std::string &getUsername() const;
        const std::string &getHostname() const;
        const std::string &getRealname() const;
        std::string getPrefix() const;
        bool isRegistered() const;
        //rida
        bool hasPassAccepted() const;

        // Setters
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setHostname(const std::string &hostname);
        void setRealname(const std::string &realname);
        void setRegistered(bool isRegistered);
        //rida
        void setPassAccepted(bool passAccepted); 

};

#endif