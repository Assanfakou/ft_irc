#include "commands.hpp"
/*
** Privmsg command implementation here 
**
** Server &server: The server instance
** Client &sender: The client who sent the PRIVMSG command
** const Message &msg: The parsed message containing the command and parameters
**
** It stil needd to implement the #chanel users.
*/

void privmsg(Server &server, Client &sender, const Message &msg)
{
    Reply reply;
    if (msg.getParams().empty())
    {
        server.sendMessageToClient(sender.getFd(), reply.needMoreParams(sender));
        return;
    }
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
    {
        server.sendMessageToClient(sender.getFd(), reply.noSuchNick(sender));
        return;
    }
    if (msg.getParams().size() < 2)
    {
        server.sendMessageToClient(sender.getFd(), reply.noTextToSend(sender));
        return;
    }
    std::string message = msg.getParameter(1);
    std::vector<Client *> receiverClients = server.getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
            {
                server.sendMessageToClient(sender.getFd(), reply.cantSendToSelf(sender));
                continue;
            }
            server.sendMessageToClient(receiverClient->getFd(), reply.generateMEssage(sender, msg));
        }
        return ;
    }
    else
    {
            server.sendMessageToClient(sender.getFd(), reply.noSuchNick(sender));
            return ;
    }
}

/*
**
** Notice command implementation **
** Server &server: The server instance
** Client &sender: The client who sent the NOTICE command
** const Message &msg: The parsed message containing the command and parameters
**
*/
void notice(Server &server, Client &sender, const Message &msg)
{
    Reply reply;
    if (msg.getParams().empty())
        return;
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
        return;
    if (msg.getParams().size() < 2)
        return;
    std::string message = msg.getParameter(1);
    std::vector<Client *> receiverClients = server.getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
                continue;
            server.sendMessageToClient(receiverClient->getFd(), reply.generateMEssage(sender, msg));
        }
        return ;
    }
    else
        return;
}

/*
** Command	            Expected behavior
** WHO	                461 ERR_NEEDMOREPARAMS
** WHO Unknown	        315 RPL_ENDOFWHO
** WHO #channel	        Return channel members
** WHO #unknown	        315 RPL_ENDOFWHO
*/

void who(Server &server, Client &sender, const Message &msg)
{
    Reply reply;
    if (msg.getParams().empty())
    {
        server.sendMessageToClient(sender.getFd(), reply.whoStartMessage(sender));
        server.sendMessageToClient(sender.getFd(), reply.needMoreParams(sender));
        server.sendMessageToClient(sender.getFd(), reply.whoEndMessage(sender));
        return;
    }
    std::string targetNickname = msg.getParameter(0);
    if (!targetNickname.empty())
    {
        Client *targetClient = server.getClientByNickname(targetNickname);
        server.sendMessageToClient(sender.getFd(), reply.whoMessage(*targetClient));
    }
}