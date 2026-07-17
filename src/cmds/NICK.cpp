#include "../../include/Server.hpp"

std::string Server::getNickname(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "NICK")
    {
        if (command.size() <= 5)
            return "";
        std::string nickname = command.substr(5);
        return nickname;
    }
    return "";
}
/*
** 
** This function will be probably a server member function
**
*/

void nickHandler(Client &client, Server &server, const std::string nickName)
{
        if (!nickName.empty())
        {
            client.setNickname(nickName);
            server. sendMessageToClient(client.getFd(), "Nickname saved: " + client.getNickname() + "\r\n");
            server.tryRegister(client);
        }
        else
        {
            server.sendMessageToClient(client.getFd(), needMoreParams(server));
            return ;
        }
}