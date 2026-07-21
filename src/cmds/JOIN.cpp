#include "../../include/Server.hpp"

JoinInfo Server::getJoin(std::string command)
{
    JoinInfo info;

    if (command.size() >= 4 && command.substr(0, 4) == "JOIN")
    {
        if (command.size() <= 5)
            return info;

        std::string value = command.substr(5);

        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.Password = value.substr(i + 1);
                return info;
            }
        }

        // No password provided
        info.channel = value;
    }

    return info;
}

void Server::addMemberTo_Channel(const Message &msg, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(1)); //now point to channel object (we can get the first elemnt or second (map))
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
    if (it->second.isPasswordEnabled())
    {
        if (msg.getParameter(2) == it->second.getPassword())
            std::cout << "Password Accepted" << std::endl;
        else
        {
            std::cout << "The password you have provided is incorrect" << std::endl;
            return;
        }
    }
    if (it->second.isUserLimitEnabled())
    {
        if (it->second.getMemberCount() >= it->second.getUserLimit())
        {
            std::cout << "The channel is full" << std::endl;
            return;
        }
    }

    it->second.addMember(fd);
    std::cout << "Client added to channel: " << msg.getParameter(1) << std::endl;
}

void Server::check_Channels_and_addMember_to_Channel(const Message &msg, Client &client)
{
    if (_channels.find(msg.getParameter(1)) == _channels.end()) //if we reach the end and we don't get the name of channel
    {
        if (!msg.getParameter(1).empty())
        {
            _channels.insert(std::make_pair(msg.getParameter(1), Channel(msg.getParameter(1)))); //pass name and creates a Channel object using constructor
            std::cout << "Channel created: " << msg.getParameter(1) << std::endl;
            
            addMemberTo_Channel(msg, client);
            std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(1));
            if (it != _channels.end())
                it->second.addOperator(client.getFd());
        }
    }
    else
        addMemberTo_Channel(msg, client);
}
