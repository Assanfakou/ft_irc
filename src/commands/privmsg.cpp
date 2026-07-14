#include "commands.hpp"

void privmsg(Server &server, Client &sender, const Message &msg)
{
    if (msg.getParams().empty())
    {
        std::string errorMessage = ":Server 411 " + sender.getNickname() + " " + msg.getParameter(0) + ": ERR_NORECIPIENT\r\n";
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