#include "../../include/Server.hpp"

const std::string& Server::getPassword() const
{
    return this->_password;
}

void Server::passHandler(Client &client, const Message &msg)
{
    if (client.hasPassAccepted())
        return ;
    if (msg.getParameter(0).empty())
    {
        sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
        return;
    }
    if (getPassword() != msg.getParameter(0))
    {
        sendMessageToClient(client.getFd(), wrongPassword(*this));
        return;
    }
    client.setPassAccepted(true);
    sendMessageToClient(client.getFd(), passwordAccepted(*this));
}
