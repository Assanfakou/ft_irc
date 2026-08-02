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

void privmsg(Server &server, Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        server.sendMessageToClient(sender.getFd(), clientNotRegestred(server));
        return ;
    }
    if (msg.getParams().empty())
    {
        server.sendMessageToClient(sender.getFd(), needMoreParams(server, sender, msg));
        return;
    }
    std::string receiver = msg.getParameter(0);
    if (receiver.empty())
    {
        server.sendMessageToClient(sender.getFd(), noSuchNick(server, sender, receiver));
        return;
    }
    if (msg.getParams().size() < 2)
    {
        server.sendMessageToClient(sender.getFd(), noTextToSend(server));
        return;
    }
    if (receiver[0] == '#')
    {
        Channel *channel = server.getChanel(receiver);
        if (channel != NULL)
            server.broadcastToChanel(*channel, sender, generateMEssage(sender, msg));
        else
            server.sendMessageToClient(sender.getFd(), noSuchChannel(server, sender, msg));
        return ;
    }
    std::vector<Client *> receiverClients = server.getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
            {
                server.sendMessageToClient(sender.getFd(), cantSendToSelf(server));
                continue;
            }
            server.sendMessageToClient(receiverClient->getFd(), generateMEssage(sender, msg));
        }
        return ;
    }
    else
    {
        server.sendMessageToClient(sender.getFd(), noSuchNick(server, sender, msg.getParameter(0)));
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

void notice(Server &server, Client &sender, const Message &msg)
{
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        server.sendMessageToClient(sender.getFd(), clientNotRegestred(server));
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
        // i should skip the # character
        Channel *channel = server.getChanel(receiver);
        server.broadcastToChanel(*channel, sender, generateMEssage(sender, msg));
        return ;
    }
    std::vector<Client *> receiverClients = server.getClientsByNickname(receiver);
    if (!receiverClients.empty())
    {
        for (std::vector<Client *>::iterator it = receiverClients.begin(); it != receiverClients.end(); ++it)
        {
            Client *receiverClient = *it;
            if (receiverClient->getFd() == sender.getFd())
                continue;
            server.sendMessageToClient(receiverClient->getFd(), generateMEssage(sender, msg));
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
    if (!sender.hasPassAccepted() || !sender.isRegistered())
    {
        server.sendMessageToClient(sender.getFd(),
            clientNotRegestred(server));
        return;
    }

    if (msg.getParams().empty())
    {
        server.sendMessageToClient(sender.getFd(),
            needMoreParams(server, sender, msg));
        return;
    }

    std::string target = msg.getParameter(0);

    if (target[0] == '#')
    {
        Channel *channel = server.getChanel(target);

        if (!channel)
        {
            server.sendMessageToClient(sender.getFd(),
                ":" + server.getServerName()
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
            Client *member = server.getClient(members[i]);

            if (!member)
                continue;

            std::string reply =
                ":" + server.getServerName()
                + " 352 "
                + sender.getNickname()
                + " "
                + channel->getName()
                + " "
                + member->getUsername()
                + " "
                + member->getHostname()
                + " "
                + server.getServerName()
                + " "
                + member->getNickname()
                + " H :0 "
                + member->getRealname()
                + "\r\n";

            server.sendMessageToClient(sender.getFd(), reply);
        }

        server.sendMessageToClient(sender.getFd(),
            ":" + server.getServerName()
            + " 315 "
            + sender.getNickname()
            + " "
            + channel->getName()
            + " :End of WHO list\r\n");

        return;
    }

    Client *targetClient =
        server.getClientByNickname(target);

    if (targetClient)
    {
        std::string reply =
            ":" + server.getServerName()
            + " 352 "
            + sender.getNickname()
            + " * "
            + targetClient->getUsername()
            + " "
            + targetClient->getHostname()
            + " "
            + server.getServerName()
            + " "
            + targetClient->getNickname()
            + " H :0 "
            + targetClient->getRealname()
            + "\r\n";

        server.sendMessageToClient(sender.getFd(), reply);
    }

    server.sendMessageToClient(sender.getFd(),
        ":" + server.getServerName()
        + " 315 "
        + sender.getNickname()
        + " "
        + target
        + " :End of WHO list\r\n");
}

// void who(Server &server, Client &sender, const Message &msg)
// {
//     if (!sender.hasPassAccepted() && !sender.isRegistered())
//     {
//         server.sendMessageToClient(sender.getFd(), clientNotRegestred(server));
//         return ;
//     }
//     if (msg.getParams().empty())
//     {
//         server.sendMessageToClient(sender.getFd(), whoStartMessage(server));
//         server.sendMessageToClient(sender.getFd(), needMoreParams(server, sender, msg));
//         server.sendMessageToClient(sender.getFd(), whoEndMessage(server));
//         return;
//     }
//     if (msg.getParameter(0) == "*")
//     {
//         server.sendMessageToClient(sender.getFd(), whoStartMessage(server));
//         server.listAllUsers(sender);
//         server.sendMessageToClient(sender.getFd(), whoEndMessage(server));
//         return ;
//     }
//     std::string targetNickname = msg.getParameter(0);
//     if (!targetNickname.empty())
//     {
//         Client *targetClient = server.getClientByNickname(targetNickname);
//         server.sendMessageToClient(sender.getFd(), whoStartMessage(server));
//         if (!targetClient)
//             server.sendMessageToClient(sender.getFd(), whoEndMessage(server));
//         else
//         {
//             server.sendMessageToClient(sender.getFd(), whoMessage(server, *targetClient));
//             server.sendMessageToClient(sender.getFd(), whoEndMessage(server));
//         }
//     }
// }