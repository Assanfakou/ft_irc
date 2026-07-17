#include "../../include/Server.hpp"

std::string Server::getPart(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "PART")
    {
        if (command.size() <= 5)
            return "";
        std::string value = command.substr(5);
        return value;
    }
    return "";
}

void Channel::leaveChannel(std::string channelName, int fd)
{
    std::vector<int>::iterator it;
    for (it = _members.begin(); it != _members.end(); it++)
    {
        if (*it == fd)
        {
            _members.erase(it);
            std::cout << "Client leaves channel " <<  channelName << std::endl;
            break;
        }  
    }
}

void Server::clientLeaveChannel(std::string channelName, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channelName); 
    if (it != _channels.end())
    {
        int fd = client.getFd();
        if (it->second.isMember(fd))
            it->second.leaveChannel(channelName, fd);
    }
    else
    {
        std::cout << "Client is not a member of the specified channel" << std::endl;
        return;
    }
}
