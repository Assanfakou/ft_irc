#include "../../include/Server.hpp"
/*
** Privmsg command implementation here 
**
** Server &server: The server instance
** Client &sender: The client who sent the PRIVMSG command
** const Message &msg: The parsed message containing the command and parameters
**
** It stil needd to implement the #chanel users.
*/

void Server::names(Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        sendMessageToClient(sender.getFd(), clientNotRegestred(*this));
        return ;
    }
    if (msg.getParameter(0).empty())
    {
        std::map<std::string, Channel>::iterator iter = _channels.begin();
        for (; iter != _channels.end(); iter++)
        {
            sendMessageToClient(sender.getFd(), namesWhenJoin(*this, sender, iter->second));
            sendMessageToClient(sender.getFd(), endOfNamesList(*this, sender, iter->second));
        }
        return ;
    }
    std::string reciever = msg.getParameter(0);
    Channel *channel = getChanel(reciever);
    if (channel == NULL)
    {
        sendMessageToClient(sender.getFd(), ":" + getServerName() + " 366 " + sender.getNickname() + " " + reciever + ":End of /NAMES List");
        return ;
    }
    if (msg.getParameter(0)[0] == '#')
    {
        sendMessageToClient(sender.getFd(), namesWhenJoin(*this, sender, *channel));
        sendMessageToClient(sender.getFd(), endOfNamesList(*this, sender, *channel));
    }
    return ;
}

void Server::privmsg(Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        sendMessageToClient(sender.getFd(), clientNotRegestred(*this));
        return ;
    }
    if (msg.getParams().empty())
    {
        sendMessageToClient(sender.getFd(), needMoreParams(*this, sender, msg));
        return;
    }
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
    {
        sendMessageToClient(sender.getFd(), noSuchNick(*this, sender, receiver));
        return;
    }
    if (msg.getParams().size() < 2)
    {
        sendMessageToClient(sender.getFd(), noTextToSend(*this));
        return;
    }
    if (receiver[0] == '#')
    {
        Channel *channel = getChanel(receiver);
        if (channel != NULL)
        {
            if (channel->isMember(sender.getFd()))
                broadcastToChanel(*channel, sender, generateMEssage(sender, msg));
            else
                sendMessageToClient(sender.getFd(), notOnChannel(*this, sender, msg));
        }
        else
            sendMessageToClient(sender.getFd(), noSuchChannel(*this, sender, msg));
        return ;
    }
    std::vector<Client *> receiverClients = getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
            {
                sendMessageToClient(sender.getFd(), cantSendToSelf(*this));
                continue;
            }
            sendMessageToClient(receiverClient->getFd(), generateMEssage(sender, msg));
        }
        return ;
    }
    else
    {
        sendMessageToClient(sender.getFd(), noSuchNick(*this, sender, msg.getParameter(0)));
        return;
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

void Server::notice(Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        sendMessageToClient(sender.getFd(), clientNotRegestred(*this));
        return ;
    }
    if (msg.getParams().empty())
        return;
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
        return;
    if (msg.getParams().size() < 2)
        return;
    if (receiver[0] == '#')
    {
        Channel *channel = getChanel(receiver);
        if (channel != NULL && channel->isMember(sender.getFd()))
            broadcastToChanel(*channel, sender, generateMEssage(sender, msg));
        return ;
    }
    std::vector<Client *> receiverClients = getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
                continue;
            sendMessageToClient(receiverClient->getFd(), generateMEssage(sender, msg));
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

void Server::who(Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        sendMessageToClient(sender.getFd(), clientNotRegestred(*this));
        return;
    }

    if (msg.getParams().empty())
    {
        sendMessageToClient(sender.getFd(), needMoreParams(*this, sender, msg));
        return;
    }
    std::string target = msg.getParameter(0);

    if (target[0] == '#')
    {
        Channel *channel = getChanel(target);

        if (!channel)
        {
            sendMessageToClient(sender.getFd(),
                ":" + getServerName()
                + " 315 "
                + sender.getNickname()
                + " "
                + target
                + " :End of WHO list\r\n");
            return;
        }

        std::vector<int> &members = channel->getMembers();

        for (size_t i = 0; i < members.size(); i++)
        {
            Client *member = getClient(members[i]);

            if (!member)
                continue;

            std::string reply =
                ":" + getServerName()
                + " 352 "
                + sender.getNickname()
                + " "
                + channel->getName()
                + " "
                + member->getUsername()
                + " "
                + member->getHostname()
                + " "
                + getServerName()
                + " "
                + member->getNickname()
                + " H :0 "
                + member->getRealname()
                + "\r\n";

            sendMessageToClient(sender.getFd(), reply);
        }

        sendMessageToClient(sender.getFd(),
            ":" + getServerName()
            + " 315 "
            + sender.getNickname()
            + " "
            + channel->getName()
            + " :End of WHO list\r\n");

        return;
    }

    Client *targetClient =
        getClientByNickname(target);

    if (targetClient)
    {
        std::string reply =
            ":" + getServerName()
            + " 352 "
            + sender.getNickname()
            + " * "
            + targetClient->getUsername()
            + " "
            + targetClient->getHostname()
            + " "
            + getServerName()
            + " "
            + targetClient->getNickname()
            + " H :0 "
            + targetClient->getRealname()
            + "\r\n";

        sendMessageToClient(sender.getFd(), reply);
    }

    sendMessageToClient(sender.getFd(),
        ":" + getServerName()
        + " 315 "
        + sender.getNickname()
        + " "
        + target
        + " :End of WHO list\r\n");
}