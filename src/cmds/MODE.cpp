#include "../../include/Server.hpp"

void Server::setMode(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() || !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));
    std::cout << "channel Name here : " << msg.getParameter(0) << std::endl;
    
    if (it == _channels.end())
    {
        sendMessageToClient(client.getFd(), noSuchChannel(*this, client, msg));
        return;
    }
    //
    if (msg.getParams().size() == 1)
    {
        std::string modes = "+";

        if (it->second.isInviteOnly())
            modes += "i";

        if (it->second.isTopicRestricted())
            modes += "t";

        if (it->second.isPasswordEnabled())
            modes += "k";

        if (it->second.isUserLimitEnabled())
            modes += "l";

        sendMessageToClient(client.getFd(),
            ":" + getServerName()
            + " 324 "
            + client.getNickname()
            + " "
            + it->second.getName()
            + " "
            + modes
            + "\r\n");

        return;
    }
    if (!it->second.isOperator(client.getFd()))
    {
        sendMessageToClient(client.getFd(), chanOpPrivsNeeded(*this, msg));
        return;
    }
    if (msg.getParameter(1) == "+i")
    {
        it->second.setInviteOnly(true);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "-i")
    {
        it->second.setInviteOnly(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "+t")
    {
        it->second.setTopicRestricted(true);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "-t")
    {
        it->second.setTopicRestricted(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "+k")
    {
        it->second.setPasswordEnabled(true);
        it->second.setPassword(msg.getParameter(2));
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "-k")
    {
        it->second.setPasswordEnabled(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "+l")
    {
        int num;
        it->second.setUserLimitEnabled(true);
        if (!msg.getParameter(2).empty())
        {
            std::string numb = msg.getParameter(2);
            int i = 0;
            while (numb[i])
            {
                std::cout << IRC_RED << numb[i] << std::endl << IRC_RESET;
                if (std::isdigit(numb[i]))
                    i++;
                else
                    return;
            }
            num = std::atoi(msg.getParameter(2).c_str());
        }
        else
        {
            sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
            return;
        }
        it->second.setUserLimit(num);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    if (msg.getParameter(1) == "-l")
    {
        it->second.setUserLimitEnabled(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
        sendMessageToClient(client.getFd(), modeMessage(client, msg));
        return ;
    }
    std::map<int, Client>::iterator it2;

    for (it2 = _clients.begin(); it2 != _clients.end(); it2++)
    {
        if (it2->second.getNickname() == msg.getParameter(2))
        {
            int targetFd = it2->second.getFd();
            if (it->second.isMember(targetFd))
            {
                if (msg.getParameter(1) == "+o")
                    it->second.addOperator(targetFd);
                if (msg.getParameter(1) == "-o")
                    it->second.removeOperator(targetFd);
                broadcastToChanel(it->second, client, modeMessage(client, msg));
                sendMessageToClient(client.getFd(), modeMessage(client, msg));
                return ;
            }
            else
            {
                sendMessageToClient(client.getFd(), userNotInChannel(*this, client, msg));
                return ;
            }
        }
    }
    sendMessageToClient(client.getFd(), noSuchNick(*this, client, msg.getParameter(2)));
    return;
}
