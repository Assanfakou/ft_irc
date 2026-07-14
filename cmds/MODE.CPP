#include "../include/Server.hpp"


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
            mode_info.Operator = mode_info.mode.substr(i + 1);
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

    std::map<int, Client>::iterator it2;

    for (it2 = _clients.begin(); it2 != _clients.end(); ++it2)
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
