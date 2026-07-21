#include "../../include/Server.hpp"

void Server::addMemberTo_Channel(const Message &msg, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0)); //now point to channel object (we can get the first elemnt or second (map))
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
        if (msg.getParameter(1) == it->second.getPassword())
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
    std::cout << "Client added to channel: " << msg.getParameter(0) << std::endl;
    Channel *reciever = getChanel(msg.getParameter(0));
    sendMessageToClient(client.getFd(), joinChannel(*this, client, msg.getParameter(0)));
    sendMessageToClient(client.getFd(), topicWhenJoin(*this, client, *reciever));
    sendMessageToClient(client.getFd(), namesWhenJoin(*this, client, *reciever));
    sendMessageToClient(client.getFd(), endOfNamesList(*this, client, *reciever));
    broadcastToChanel(*reciever, client, joinChannel(*this, client, msg.getParameter(0)));

}

void Server::check_Channels_and_addMember_to_Channel(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    if (msg.getParameter(0)[0] != '#')
    {
        sendMessageToClient(client.getFd(), notValidChanelName(*this));
        return ;
    }
    if (_channels.find(msg.getParameter(0)) == _channels.end()) //if we reach the end and we don't get the name of channel
    {
        if (!msg.getParameter(0).empty())
        {
            _channels.insert(std::make_pair(msg.getParameter(0), Channel(msg.getParameter(0)))); //pass name and creates a Channel object using constructor
            std::cout << "Channel created: " << msg.getParameter(0) << std::endl;
            
            addMemberTo_Channel(msg, client);
            std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));
            if (it != _channels.end())
                it->second.addOperator(client.getFd());
        }
    }
    else
        addMemberTo_Channel(msg, client);
}
