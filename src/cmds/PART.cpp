#include "../../include/Server.hpp"

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
/******Part
* - 1 channel name 
* - :folowed by a godby message
*
*/
void Server::clientLeaveChannel(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));
    if (it != _channels.end())
    {
        int fd = client.getFd();
        if (it->second.isMember(fd))
        {
            it->second.leaveChannel(msg.getParameter(0), fd);
            broadcastToChanel(it->second, client, partMessage(client, msg));
            return ;
        }
        else
        {
            sendMessageToClient(client.getFd(), notOnChannel(*this, client, msg));
            return ;
        }
    }
    else
    {
        sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
        return;
    }
}
