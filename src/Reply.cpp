#include "../include/Reply.hpp"

Reply::Reply() {}

std::string Reply::generateMEssage(const Client &client, const Message &msg)
{
    std::string message = ":" + client.getPrefix() + " " + msg.getCommand();
    for (size_t i = 0; i < msg.getParams().size(); ++i)
    {
        message += " " + msg.getParameter(i);
    }
    message += "\r\n";
    return message;
}

std::string Reply::noTextToSend(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 412 : ERR_NOTEXTTOSEND\r\n";
    return message;
}

std::string Reply::noSuchNick(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 401 : ERR_NOSUCHNICK\r\n";
    return message;
}

std::string Reply::needMoreParams(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 461 : ERR_NEEDMOREPARAMS\r\n";
    return message;
}

std::string Reply::cantSendToSelf(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 404 : ERR_CANTSENDTOSELF\r\n";
    return message;
}

std::string Reply::unknownCommand(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 421 : ERR_UNKNOWNCOMMAND\r\n";
    return message;
}

std::string Reply::welcomeMessage(const Client &client)
{
    std::string message = ":" + client.getPrefix() + " 001 : Welcome to the IRC server!\r\n";
    return message;
}
