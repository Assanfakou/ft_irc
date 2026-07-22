#include "../../include/Server.hpp"

/*

** invite takess 
** arg 0 -user
** arg 1 - channel

*/
void Server::compare_nickname_and_inviteClient(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<int, Client>::iterator it;

    if (msg.getParameter(1).empty() || msg.getParameter(0).empty())
    {
        sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
        return ;
    }
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.getNickname() == msg.getParameter(0))
        {
            int targetFd = it->second.getFd();

            std::map<std::string, Channel>::iterator it2 = _channels.find(msg.getParameter(1));
            
            if (it2 == _channels.end())
            {
                /* this should be noSuchChannel */
                sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
                return;
            }
            if (it2->second.isMember(targetFd))
            {
                sendMessageToClient(client.getFd(), ":server 443 clientnick targetnick #channel :is already on channel\r\n");
                return;
            }

            if (it2->second.isInvited(targetFd))
            {
                sendMessageToClient(client.getFd(), ":server client already invited\r\n");
                return;
            }
            /*should make everything it's error */
            if (it2->second.isMember(client.getFd()) && it2->second.isOperator(client.getFd()))
            {
                it2->second.addInvitedClient(targetFd);
                sendMessageToClient(client.getFd(), ":server 341 clientnick target #channel\r\n");
                sendMessageToClient(targetFd, ":prefixsender Command target :#channel\r\n");
            }
            else
                sendMessageToClient(client.getFd(), ": server 442 you are not in channel 482 you are not channel opearator\r\n");
            return;
        }
        else
        {
            sendMessageToClient(client.getFd(), noSuchNick(*this, client, msg.getParameter(0)));
            return;
        }
    }
}