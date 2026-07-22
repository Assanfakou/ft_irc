#include "../../include/Server.hpp"

void Server::showTopic(const Message &msg, Client &client)
{

    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));

    if (it == _channels.end())
    {
        sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
        return;
    }

    if (!msg.getParameter(1).empty())
    {
        if (it->second.isTopicRestricted())
        {
            sendMessageToClient(client.getFd(), chanOpPrivsNeeded(*this, msg));
            return;
        }
        if (it->second.isOperator(client.getFd()))
        {
            it->second.setTopic(msg.getParameter(1));
            broadcastToChanel(it->second, client, topicMessage(client, msg));
            return ;
        }
    }
    else
    {
        sendMessageToClient(client.getFd(), topicReply(*this, client, it->second));
        return ;
    }
}
