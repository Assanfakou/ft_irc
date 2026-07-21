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
    
    if (it == _channels.end())
    {
        std::cout << "Channel not found" << std::endl;
        return;
    }
    if (!it->second.isOperator(client.getFd()))
    {
        std::cout << "You don't have permission to change channel modes." << std::endl;
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
        std::cout << "The password has been set successfully." << std::endl;
    }
    if (msg.getParameter(1) == "-k")
    {
        it->second.setPasswordEnabled(false);
        std::cout << "The password has been removed successfully." << std::endl;
    }
    if (msg.getParameter(1) == "+l")
    {
        int num;
        it->second.setUserLimitEnabled(true);
        if (!msg.getParameter(2).empty())
            num = std::atoi(msg.getParameter(2).c_str());
        else
            return ; /*
                    ** notice that sends 461 NEEDMORE params 
                    ** and if it gives a non non numbers it gives 696 invalide mode number
                     */
        it->second.setUserLimit(num);

        std::cout << "The limit of the channel has been set to " 
        << it->second.getUserLimit() << std::endl;
    }
    if (msg.getParameter(1) == "-l")
    {
        it->second.setUserLimitEnabled(false);
        std::cout << "The limit of channel has been removed successfully." << std::endl;
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
            }
        }
    }

    return;
}
