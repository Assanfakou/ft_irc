#include "../../include/Server.hpp"

/*
** 
** This function will be probably a member function
**
*/

bool firstNickCharacter(char c)
{
    if (std::isalpha(c))
        return true;
    if (c == '[' || c == ']' || c == '\\' || c == '^' || c ==  '{' || c == '}' || c == '`')
        return true;
    return false;
}

char ircToUpper(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');

    switch (c)
    {
        case '{': return '[';
        case '}': return ']';
        case '|': return '\\';
        case '^': return '~';
        default:  return c;
    }
}
bool isValidCharachter(char c)
{
    if (std::isalnum(c))
        return true;
    if (c == '[' || c == ']' || c == '\\' || c == '^' || c ==  '{' || c == '}' || c == '`'
        || c == '-' || c == '_' || c == '|') 
        return true;
    return false;
}

bool compareNick(const std::string &a, const std::string &b)
{
    if (a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); ++i)
    {
        if (ircToUpper(a[i]) != ircToUpper(b[i]))
            return false;
    }
    return true;
}

bool checkNick(const std::string &nick)
{
    for (size_t i = 1; i < nick.size(); i++)
    {
        if (!isValidCharachter(nick[i]))
            return false;
    }
    return true;
}


void Server::nickHandler(Client &client, const Message &msg)
{
    if (!client.getPassAccepted())
        return ;
    if (msg.getParameter(0).empty())
    {
        sendMessageToClient(client.getFd(), needMoreParams(*this, client, msg));
        return;
    }
    if (msg.getParameter(0).size() > 9)
    {
        sendMessageToClient(client.getFd(), ":" + getServerName() + " 432 * " + msg.getParameter(0) + " :Erroneous Nickname\r\n");
         
        return;
    }
    if (!firstNickCharacter(msg.getParameter(0)[0]))
    {
        sendMessageToClient(client.getFd(), ":" + getServerName() + " 432 * " + msg.getParameter(0) + " :Erroneous Nickname\r\n");
        return;
    }
    if (!checkNick(msg.getParameter(0)))
    {
        sendMessageToClient(client.getFd(), "432 * " + msg.getParameter(0) + " :Erroneous Nickname\r\n");
        return;
    }
    Client *fakeClient = getClientByNickname(msg.getParameter(0));
    if (fakeClient && fakeClient->getFd() != client.getFd())
    {
        sendMessageToClient(client.getFd(), "this nickName is taken\r\n");
        return;
    }
    std::string oldPrefix = client.getPrefix();
    client.setNickname(msg.getParameter(0));
    sendMessageToClient(client.getFd(), ":" + oldPrefix + " NICK :" + client.getNickname() + "\r\n");
    // broadcast the message to all users
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->second.isMember(client.getFd()))
            broadcastToChanel(it->second, client, ":" + oldPrefix + " NICK :" + client.getNickname() + "\r\n");
    }
}