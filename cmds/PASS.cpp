#include "../include/Server.hpp"

std::string Server::getPassword(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "PASS")
    {
        if (command.size() <= 5)
            return "";
        std::string password = command.substr(5);
        return password;
    }
    return "";
}