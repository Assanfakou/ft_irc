#include "../../include/Server.hpp"

void Server::showTopic(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() || !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));

    if (it == _channels.end())
    {
        sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
        return;
    }
    if (msg.getParameter(1).empty())
    {
        if (it->second.getTopic().empty())
            sendMessageToClient(client.getFd(), noTopic(*this, client, msg));
        else
            sendMessageToClient(client.getFd(), topicReply(*this, client, it->second));
        return;
    }
    if (it->second.isTopicRestricted() && !it->second.isOperator(client.getFd()))
    {
        sendMessageToClient(client.getFd(), chanOpPrivsNeeded(*this, msg));
        return;
    }
    it->second.setTopic(msg.getParameter(1));
    broadcastToChanel(it->second, client, topicMessage(client, msg));
    sendMessageToClient(client.getFd(), topicMessage(client, msg));
}