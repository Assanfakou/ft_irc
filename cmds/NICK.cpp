#include "../include/Server.hpp"

std::string Server::getNickname(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "NICK")
    {
        if (command.size() <= 5)
            return "";
        std::string nickname = command.substr(5);
        return nickname;
    }
    return "";
}