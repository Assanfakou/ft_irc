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
    if (msg.getParams().empty())
    {
        std::string errorMessage = ":Server 461 " + sender.getNickname() + " : ERR_NEEDMOREPARAMS\r\n";
        server.sendMessageToClient(sender.getFd(), errorMessage);
        return;
    }
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
    {
        std::string errorMessage = ":Server 401 : ERR_NOSUCHNICK\r\n";
        server.sendMessageToClient(sender.getFd(), errorMessage);
        return;
    }
    if (msg.getParams().size() < 2)
    {
        std::string errorMessage = ":Server 412 : ERR_NOTEXTTOSEND\r\n";
        server.sendMessageToClient(sender.getFd(), errorMessage);
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
                std::string errorMessage = ":Server 404 : ERR_CANTSENDTOSELF\r\n";
                server.sendMessageToClient(sender.getFd(), errorMessage);
                continue;;
            }
            std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + receiverClient->getNickname() + " :" + message + "\r\n";
            server.sendMessageToClient(receiverClient->getFd(), fullMessage);
        }
        return ;
    }
    else
    {
            std::string errorMessage = ":Server 401 : ERR_NOSUCHNICK\r\n";
            server.sendMessageToClient(sender.getFd(), errorMessage);
            return ;
    }
}

void notice(Server &server, Client &sender, const Message &msg)
{
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
                continue;;
            std::string fullMessage = ":" + sender.getPrefix() + " NOTICE " + receiverClient->getNickname() + " :" + message + "\r\n";
            server.sendMessageToClient(receiverClient->getFd(), fullMessage);
        }
        return ;
    }
    else
        return;
}
void whois(Server &server, Client &sender, const Message &msg)
{
    if (msg.getParams().empty())
    {
        std::string errorMessage = ":Server 461 " + sender.getNickname() + " : ERR_NEEDMOREPARAMS\r\n";
        server.sendMessageToClient(sender.getFd(), errorMessage);
        return;
    }
    std::string targetNickname = msg.getParameter(0);
    if (!targetNickname.empty())
    {
        Client *receiverClient = server.getClientByNickname(targetNickname);
        std::string messageToSend = ":" + sender.getPrefix() + " WHO ";
        server.sendMessageToClient(receiverClient->getFd(), messageToSend);
    }
}