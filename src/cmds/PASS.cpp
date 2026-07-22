#include "../../include/Server.hpp"

const std::string& Server::getPassword() const
{
    return this->_password;
}

void passHandler(Server &server, Client &client, const Message &msg)
{
    if (!msg.getParameter(0).empty())
    {
        if (server.getPassword() == msg.getParameter(0))
        {
            client.setPassAccepted(true);
            server.sendMessageToClient(client.getFd(), passwordAccepted(server));
            server.sendMessageToClient(client.getFd(), welcomeMessage(server, client));
            server.tryRegister(client);
            return;
        }
        else
            server.sendMessageToClient(client.getFd(), ":" + server.getServerName() + "  Wrong Password\r\n");
    }
    else
        server.sendMessageToClient(client.getFd(), needMoreParams(server, client, msg));
    return;
}