#include "../../include/Server.hpp"

/*
** 
** This function will be probably a server member function
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
    if (c == '[' || c == ']' || c == '\\' || c == '^' || c ==  '{' || c == '}' || c == '`')
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

void nickHandler(Client &client, Server &server, const Message &msg)
{
    if (!msg.getParameter(0).empty())
    {
        if (msg.getParameter(0).size() > 9)
        {
            server.sendMessageToClient(client.getFd(), "Long NickName\r\n");
            return;
        }
        Client *fakeClient = server.getClientByNickname(msg.getParameter(0));
        if (!fakeClient)
        {
            if (!firstNickCharacter(msg.getParameter(0)[0]))
            {
                server.sendMessageToClient(client.getFd(), "Firstcharachter invalid\r\n");
                return;
            }
            if (!checkNick(msg.getParameter(0)))
            {
                server.sendMessageToClient(client.getFd(), "charachterNotvalid\r\n");
                return ;
            }
            client.setNickname(msg.getParameter(0));
            server.sendMessageToClient(client.getFd(), client.getPrefix() +" NICK :"+ client.getNickname() + "\r\n");
            server.tryRegister(client);
        }
        else
        {
            if (fakeClient->getFd() == client.getFd())
            {
                server.sendMessageToClient(client.getFd(), client.getPrefix() + " NICK :" + client.getNickname() + "\r\n");
                client.setNickname(msg.getParameter(0));
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