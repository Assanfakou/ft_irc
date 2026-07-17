#include "../../include/Server.hpp"

std::string Server::getJoin(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "JOIN")
    {
        if (command.size() <= 5)
            return "";
        std::string channelName = command.substr(5);
        return channelName;
    }
    return "";
}

void Server::addMemberTo_Channel(std::string channelName, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channelName); //now point to channel object (we can get the first elemnt or second (map))
    int fd = client.getFd();
    if (it->second.isMember(fd))
    {
        std::cout << "Client already in channel" << std::endl;
        return;
    }
    if (it->second.isInviteOnly())
    {
        if (!it->second.isInvited(fd))
        {
            std::cout << "You don't have permission to join the channel" << std::endl;
            return;
        }
    }
    it->second.addMember(fd);
    std::cout << "Client added to channel: " << channelName << std::endl;
}

void Server::check_Channels_and_addMember_to_Channel(std::string channelName, Client &client)
{
    if (_channels.find(channelName) == _channels.end()) //if we reach the end and we don't get the name of channel
    {
        if (!channelName.empty())
        {
            _channels.insert(std::make_pair(channelName, Channel(channelName))); //pass name and creates a Channel object using constructor
            std::cout << "Channel created: " << channelName << std::endl;
            
            addMemberTo_Channel(channelName, client);
            std::map<std::string, Channel>::iterator it = _channels.find(channelName);
            if (it != _channels.end())
                it->second.addOperator(client.getFd());
        }
    }
    else
        addMemberTo_Channel(channelName, client);
}