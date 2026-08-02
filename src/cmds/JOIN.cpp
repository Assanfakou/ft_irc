#include "../../include/Server.hpp"

void Server::addMemberTo_Channel(Channel &channel, const Message &msg, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channel.getName()); //now point to channel object (we can get the first elemnt or second (map))
    int fd = client.getFd();
    if (it->second.isMember(fd))
    {
        sendMessageToClient(client.getFd(), userAlreadyOnChannel(*this, client, msg));
        return;
    }
    if (it->second.isInviteOnly())
    {
        if (!it->second.isInvited(fd))
        {
            sendMessageToClient(client.getFd(), inviteOnlyChan(*this, client, msg));
            return;
        }
    }
    if (it->second.isPasswordEnabled())
    {
        if (msg.getParameter(1) == it->second.getPassword())
            std::cout << "Password Accepted" << std::endl;
        else
        {
            sendMessageToClient(client.getFd(), badChannelKey(*this, client, msg));
            return;
        }
    }
    if (it->second.isUserLimitEnabled())
    {
        if (it->second.getMemberCount() >= it->second.getUserLimit())
        {
            sendMessageToClient(client.getFd(), channelIsFull(*this, client, msg));
            return;
        }
    }
    it->second.addMember(fd);
    if (firstMember)
        it->second.addOperator(fd);
    /* this is for erasing the invitation */
    it->second.ereasFromInvitedVec(fd);
    /*this is for removing the channel if it is empty*/
    it->second.setEmpty(false);
    /*-----------------------------*/
    std::cout << "Client added to channel: " << channel.getName() << std::endl;
    sendMessageToClient(client.getFd(), joinChannel(*this, client, channel.getName()));
    sendMessageToClient(client.getFd(), topicWhenJoin(*this, client, channel));
    sendMessageToClient(client.getFd(), namesWhenJoin(*this, client, channel));
    sendMessageToClient(client.getFd(), endOfNamesList(*this, client, channel));
    broadcastToChanel(channel, client, joinChannel(*this, client, channel.getName()));
}

void Server::check_Channels_and_addMember_to_Channel(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    if (msg.getParameter(0).empty())
    {
        sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
        return ;
    }
    if (msg.getParameter(0).size() == 1)
    {
        if (msg.getParameter(0)[0] == '0')
        {
            removeExitedClientInChannels(client.getFd());
            return ;
        }
        else
            sendMessageToClient(client.getFd(), "bad Parameter\r\n");
        return ;
    }
    // if (msg.getParameter(0)[0] != '#')
    // {
    //     sendMessageToClient(client.getFd(), notValidChanelName(*this));
    //     return;
    // }
    std::cout << "channel names : " << msg.getParameter(0)<< std::endl;
    std::vector<Channel *> channels = getChannelsByName(msg.getParameter(0));
    for (std::vector<Channel *>::iterator iter = channels.begin(); iter != channels.end(); iter++)
    {
        firstMember = false;
        if ((*iter)->getMemberCount() == 0)
            firstMember = true;
        addMemberTo_Channel(**iter, msg, client);
    }
}
