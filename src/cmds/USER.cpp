#include "../../include/Server.hpp"

void Server::userHandler(Client &client, const Message &msg)
{
    if (client.getNickname().empty() || !client.getPassAccepted())
        return ;
    if (!msg.getParameter(0).empty())
    {
        client.setUsername(msg.getParameter(0));
        if (!msg.getParameter(3).empty())
            client.setRealname(msg.getParameter(3));
        std::cout << "Username saved: " << client.getUsername() << std::endl;
        tryRegister(client);
    }
    else
        sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
}