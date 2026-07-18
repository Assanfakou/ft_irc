#include "../../include/Server.hpp"

Inviteinfo Server::getInviteInfo(std::string command)
{
    Inviteinfo info;

    if (command.size() >= 6 && command.substr(0, 6) == "INVITE")
    {
        if (command.size() <= 7)
            return info;
        std::string value = command.substr(7);
        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.nickname = value.substr(i + 1);
                return info;
            }
        }
    }
    return info;
}
/*
**
** invite function takes arguments 1} - nickName, then  2} - chaneslName
** with the error messages should be sent to the target or the sender in case 
** of errors. << later >>
**
*/
void Server::compare_nickname_and_inviteClient(const std::string &channelName, const std::string &nickname, Client &client)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<int, Client>::iterator it;

    if (channelName.empty() || nickname.empty())
    {
        sendMessageToClient(client.getFd(), needMoreParams(*this));
        return ;
    }
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            int targetFd = it->second.getFd();

            std::map<std::string, Channel>::iterator it2 = _channels.find(channelName);
            
            if (it2 == _channels.end())
            {
                std::cout << "Channel not found" << std::endl;
                return;
            }
            
            if (it2->second.isMember(targetFd))
            {
                std::cout << "Client Already in this channel" << std::endl;
                return;
            }

            if (it2->second.isInvited(targetFd))
            {
                std::cout << "Client already invited" << std::endl;
                return;
            }
            
            if (it2->second.isMember(client.getFd()) && it2->second.isOperator(client.getFd()))
            {
                it2->second.addInvitedClient(targetFd);
                std::cout << nickname << " invited to " << channelName << std::endl;
            }
            return;
        }
    }
    std::cout << "Nickname not found" << std::endl;
}