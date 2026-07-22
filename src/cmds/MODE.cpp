#include "../../include/Server.hpp"

// bool isNumeric(const std::string& str) 
// {
//     if (str.empty()) 
//         return false;
    
//     std::istringstream iss(str); //iss : object (instance), str: argument passed to the constructor
//     size_t Buffer;
    
//     return (iss >> Buffer) && iss.eof(); //iss.eof() ensures that nothing remains after the number
// }

void Server::setMode(const Message &msg, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
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
    if (!it->second.isOperator(client.getFd()))
    {
        sendMessageToClient(client.getFd(), chanOpPrivsNeeded(*this, msg));
        return;
    }
    if (msg.getParameter(1) == "+i")
        it->second.setInviteOnly(true);
    if (msg.getParameter(1) == "-i")
        it->second.setInviteOnly(false);
    if (msg.getParameter(1) == "+t")
        it->second.setTopicRestricted(true);
    if (msg.getParameter(1) == "-t")
        it->second.setTopicRestricted(false);
    if (msg.getParameter(1) == "+k")
    {
        it->second.setPasswordEnabled(true);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
    }
    if (msg.getParameter(1) == "-k")
    {
        it->second.setPasswordEnabled(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
    }
    if (msg.getParameter(1) == "+l")
    {
        int num;
        it->second.setUserLimitEnabled(true);
        if (!msg.getParameter(2).empty())
            num = std::atoi(msg.getParameter(2).c_str());
        else
        {
            sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
            return;
        }
        /** notice that sends 461 NEEDMORE params
         ** and if it gives a non non numbers it gives 696 invalide mode number
         */
        it->second.setUserLimit(num);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
    }
    if (msg.getParameter(1) == "-l")
    {
        it->second.setUserLimitEnabled(false);
        broadcastToChanel(it->second, client, modeMessage(client, msg));
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
            }
        }
    }
    return;
}
