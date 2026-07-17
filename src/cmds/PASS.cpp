#include "../../include/Server.hpp"
#include "../../include/Reply.hpp"

const std::string& Server::getPassword() const
{
    return this->_password;
}

void passHandler(Server &server, Client &client, const std::string &password)
{
        if (!password.empty())
        {
            if (server.getPassword() == password)
            {
                client.setPassAccepted(true);
                server.sendMessageToClient(client.getFd(), ":" + server.getServerName() + " Password Accepted\r\n");
                server.tryRegister(client);
                return ;
            }
            else
                server.sendMessageToClient(client.getFd(), ":" + server.getServerName() + "  Wrong Password\r\n");
        }
        else
            server.sendMessageToClient(client.getFd(), needMoreParams(server));
        return;
}