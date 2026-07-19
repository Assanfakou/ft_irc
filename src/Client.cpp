#include "../include/Client.hpp"

Client::Client(int fd) : _fd(fd), _isRegistered(false), _passAccepted(false), _nickname(""), _username(""), _hostname(""), _realname("") {}

// Getters
int Client::getFd() const
{
    return (_fd);
}

std::string &Client::getBuffer()
{
    return (_buffer);
}

const std::string &Client::getNickname() const
{
    return (_nickname);
}

const std::string &Client::getUsername() const
{
    return (_username);
}

const std::string &Client::getHostname() const
{
    return (_hostname);
}

const std::string &Client::getRealname() const
{
    return (_realname);
}

bool Client::isRegistered() const
{
    return (_isRegistered);
}

//rida
bool Client::hasPassAccepted() const
{
    return _passAccepted;
}

// Setters
void Client::setNickname(const std::string &nickname)
{
    _nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
    _username = username;
}

void Client::setHostname(const std::string &hostname)
{
    _hostname = hostname;
}

void Client::setRealname(const std::string &realname)
{
    _realname = realname;
}

void Client::setRegistered(bool isRegistered)
{
    _isRegistered = isRegistered;
}

std::string Client::getPrefix() const
{
    return _nickname + "!" + _username + "@" + _hostname;
}

//rida
void Client::setPassAccepted(bool passAccepted)
{
    _passAccepted = passAccepted;
}