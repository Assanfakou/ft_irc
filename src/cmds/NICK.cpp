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

void nickHandler(Client &client, Server &server, const Message &msg)
{
    if (!msg.getParameter(0).empty())
    {
        Client *fakeClient = server.getClientByNickname(msg.getParameter(0));
        if (!fakeClient)
        {
            client.setNickname(msg.getParameter(0));
            server.sendMessageToClient(client.getFd(), "YOU'RE KNOWN AS : " + client.getNickname() + "\r\n");
            server.tryRegister(client);
        }
        else
        {
            if (fakeClient->getFd() == client.getFd())
            {
                client.setNickname(msg.getParameter(0));
                server.sendMessageToClient(client.getFd(), "YOU'RE KNOWN AS : " + client.getNickname() + "\r\n");
                server.tryRegister(client);
            }
            else
            {
                server.sendMessageToClient(client.getFd(), "this nickName is taken \r\n" );
                return;
            }
        }
    }
    else
    {
        server.sendMessageToClient(client.getFd(), needMoreParams(server, client, msg));
        return;
    }
}