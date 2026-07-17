#include "../../include/Server.hpp"

Kickinfo Server::getKickInfo(std::string command)
{
    Kickinfo info;

    if (command.size() >= 4 && command.substr(0, 4) == "KICK")
    {
        if (command.size() <= 5)
            return info;

        std::string value = command.substr(5);

        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.nickname = value.substr(i + 1);
                return info;
            }
        }
    }
    return info;
}

void Server::compare_nickname_and_kickClient(const std::string &channelName, const std::string &nickname, Client &client)
{
    std::map<int, Client>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            int targetFd = it->second.getFd();

            std::map<std::string, Channel>::iterator it2 = _channels.find(channelName);
            
            if (it2 == _channels.end())
            {
                std::cout << "Channel not found" << std::endl;
                return;
            }
            
            if (!it2->second.isMember(targetFd))
            {
                std::cout << "Client is not in this channel" << std::endl;
                return;
            }
            
            if (it2->second.isMember(client.getFd()) && it2->second.isOperator(client.getFd()))
            {
                it2->second.leaveChannel(channelName, targetFd);
                std::cout << nickname << " kicked from " << channelName << std::endl;
            }
            return;
        }
    }

    std::cout << "Nickname not found" << std::endl;
}