#include "../include/Server.hpp"

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

void Server::addMemberTo_Channel(JoinInfo join_info, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(join_info.channel); //now point to channel object (we can get the first elemnt or second (map))
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
        if (join_info.Password == it->second.getPassword())
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
    std::cout << "Client added to channel: " << join_info.channel << std::endl;
}

void Server::check_Channels_and_addMember_to_Channel(JoinInfo join_info, Client &client)
{
    if (_channels.find(join_info.channel) == _channels.end()) //if we reach the end and we don't get the name of channel
    {
        if (!join_info.channel.empty())
        {
            _channels.insert(std::make_pair(join_info.channel, Channel(join_info.channel))); //pass name and creates a Channel object using constructor
            std::cout << "Channel created: " << join_info.channel << std::endl;
            
            addMemberTo_Channel(join_info, client);
            std::map<std::string, Channel>::iterator it = _channels.find(join_info.channel);
            if (it != _channels.end())
                it->second.addOperator(client.getFd());
        }
    }
    else
        addMemberTo_Channel(join_info, client);
}