#include "../../include/Server.hpp"

/**
 * kick takes three parameteres 
 * 1 - channelName
 * 2 - nickName
 * 3 - :reason
**/

void Server::compare_nickname_and_kickClient(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<int, Client>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.getNickname() == msg.getParameter(1))
        {
            int targetFd = it->second.getFd();

            std::map<std::string, Channel>::iterator it2 = _channels.find(msg.getParameter(0));
            
            if (it2 == _channels.end())
            {
                sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
                return;
            }
            
            if (!it2->second.isMember(targetFd))
            {
                sendMessageToClient(client.getFd(), userNotInChannel(*this, client, msg));
                return;
            }
            
            if (it2->second.isMember(client.getFd()))
            {
                if (it2->second.isOperator(client.getFd()))
                {
                    broadcastToChanel(it2->second, client, kickMessage(client, msg));
                    sendMessageToClient(client.getFd(), kickMessage(client, msg));
                    it2->second.leaveChannel(msg.getParameter(0), targetFd);
                    return ;
                }
                else
                {
                    sendMessageToClient(client.getFd(), chanOpPrivsNeeded(*this, msg));
                    return ;
                }
            }
            else
            {
                sendMessageToClient(client.getFd(), notOnChannel(*this, client, msg));
                return ;
            }
            return;
        }
    }
    sendMessageToClient(client.getFd(), noSuchNick(*this, client, msg.getParameter(1)));
}