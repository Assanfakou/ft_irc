#include "../../include/Server.hpp"

const std::string& Server::getPassword() const
{
    return this->_password;
}

void passHandler(Server &server, Client &client, const Message &msg)
{
    if (client.hasPassAccepted())
        return ;
    if (msg.getParameter(0).empty())
    {
        server.sendMessageToClient(client.getFd(),
            needMoreParams(server, client, msg));
        return;
    }

    if (server.getPassword() != msg.getParameter(0))
    {
        server.sendMessageToClient(client.getFd(), wrongPassword(server));
        return;
    }
    client.setPassAccepted(true);

    server.sendMessageToClient(client.getFd(), passwordAccepted(server));
}
