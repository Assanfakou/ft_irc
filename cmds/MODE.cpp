#include "../include/Server.hpp"
#include <sstream>
#include <cstdlib>

Modeinfo Server::getModeInfo(std::string command)
{
    Modeinfo info;

    if (command.size() >= 4 && command.substr(0, 4) == "MODE")
    {
        if (command.size() <= 5)
            return info;

        std::string value = command.substr(5);

        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.mode = value.substr(i + 1);
                return info;
            }
        }
    }
    return info;
}

// bool isNumeric(const std::string& str) 
// {
//     if (str.empty()) 
//         return false;
    
//     std::istringstream iss(str); //iss : object (instance), str: argument passed to the constructor
//     size_t Buffer;
    
//     return (iss >> Buffer) && iss.eof(); //iss.eof() ensures that nothing remains after the number
// }

void Server::setMode(Modeinfo mode_info, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(mode_info.channel);
    
    if (it == _channels.end())
    {
        std::cout << "Channel not found" << std::endl;
        return;
    }
    if (!it->second.isOperator(client.getFd()))
    {
        std::cout << "You don't have permission to change channel modes." << std::endl;
        return;
    }
    for (size_t i = 0; i < mode_info.mode.size(); i++)
    {
        if (mode_info.mode[i] == ' ')
        {
            //the order of 0, i is matter because if you passed (0, i) first the rest is gone
            mode_info.Operator = mode_info.mode.substr(i + 1);
            mode_info.Secret = mode_info.mode.substr(i + 1);
            it->second.setPassword(mode_info.Secret);
            mode_info.userLimit = mode_info.mode.substr(i + 1);
            mode_info.mode = mode_info.mode.substr(0, i);
            break;
        }
    }
    if (mode_info.mode == "+i")
        it->second.setInviteOnly(true);
    if (mode_info.mode == "-i")
        it->second.setInviteOnly(false);
    if (mode_info.mode == "+t")
        it->second.setTopicRestricted(true);
    if (mode_info.mode == "-t")
        it->second.setTopicRestricted(false);
    if (mode_info.mode == "+k")
    {
        it->second.setPasswordEnabled(true);
        std::cout << "The password has been set successfully." << std::endl;
    }
    if (mode_info.mode == "-k")
    {
        it->second.setPasswordEnabled(false);
        std::cout << "The password has been removed successfully." << std::endl;
    }
    if (mode_info.mode == "+l")
    {
        it->second.setUserLimitEnabled(true);
        int num = std::atoi(mode_info.userLimit.c_str());
        it->second.setUserLimit(num);

        std::cout << "The limit of the channel has been set to " 
        << it->second.getUserLimit() << std::endl;
    }
    if (mode_info.mode == "-l")
    {
        it->second.setUserLimitEnabled(false);
        std::cout << "The limit of channel has been removed successfully." << std::endl;
    }

    std::map<int, Client>::iterator it2;

    for (it2 = _clients.begin(); it2 != _clients.end(); it2++)
    {
        if (it2->second.getNickname() == mode_info.Operator)
        {
            int targetFd = it2->second.getFd();
            if (it->second.isMember(targetFd))
            {
                if (mode_info.mode == "+o")
                    it->second.addOperator(targetFd);
                if (mode_info.mode == "-o")
                    it->second.removeOperator(targetFd);
            }
        }
    }

    return;
}
