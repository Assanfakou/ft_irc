#include "../../include/Server.hpp"

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