#include "../../include/Server.hpp"
#include "../../include/Message.hpp"

Modeinfo Server::getModeInfo(std::string command)
{
    Modeinfo info;

    if (command.size() >= 4 && command.substr(0, 4) == "MODE")
    {
        if (command.size() <= 5)
            return info;

        std::string value = command.substr(5);

        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.mode = value.substr(i + 1);
                return info;
            }
        }
    }
    return info;
}

void Server::setMode(const Message &msg, Client &client)
{
    const std::string &chanelName = msg.getParameter(0).substr(1);
    std::cout << "chanel name " << chanelName << std::endl;
    std::map<std::string, Channel>::iterator it = _channels.find(chanelName);

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
    // for (size_t i = 0; i < msg.getParameter(1).size(); i++)
    // {
    //     if (msg.getParameter(1)[i] == ' ')
    //     {
    //         mode_info.Operator = msg.getParameter(1).substr(i + 1);
    //         msg.getParameter(1) = mode_info.mode.substr(0, i);
    //         break;
    //     }
    // }
    if (msg.getParameter(1) == "+i")
        it->second.setInviteOnly(true);
    if (msg.getParameter(1) == "-i")
        it->second.setInviteOnly(false);
    if (msg.getParameter(1) == "+t")
        it->second.setTopicRestricted(true);
    if (msg.getParameter(1) == "-t")
        it->second.setTopicRestricted(false);

    std::map<int, Client>::iterator it2;

    for (it2 = _clients.begin(); it2 != _clients.end(); ++it2)
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
