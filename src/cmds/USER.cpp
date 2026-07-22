#include "../../include/Server.hpp"

std::string Server::getUsername(std::string command)
{
    if (command.size() >= 4 && command.substr(0, 4) == "USER")
    {
        if (command.size() <= 5)
            return "";
        std::string nickname = command.substr(5);
        return nickname;
    }
    return "";
}

void userHandler(Server &server, Client &client, const Message &msg)
{
    if (!msg.getParameter(0).empty())
    {
        client.setUsername(msg.getParameter(0));
        if (!msg.getParameter(3).empty())
            client.setRealname(msg.getParameter(3));
        std::cout << "Username saved: " << client.getUsername() << std::endl;
        server.tryRegister(client);
    }
    else
        server.sendMessageToClient(client.getFd(), needMoreParams(server, client, msg));
}