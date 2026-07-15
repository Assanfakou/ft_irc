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

std::string Reply::noTextToSend()
{
    return ":Server 412 : ERR_NOTEXTTOSEND\r\n";
}

std::string Reply::noSuchNick()
{
    return ":Server 401 : ERR_NOSUCHNICK\r\n";
}

std::string Reply::needMoreParams()
{
    return ":Server 461 : ERR_NEEDMOREPARAMS\r\n";
}

std::string Reply::cantSendToSelf()
{
    return ":Server 404 : ERR_CANTSENDTOSELF\r\n";
}

std::string Reply::unknownCommand()
{
    return ":Server 421 : ERR_UNKNOWNCOMMAND\r\n";
}

std::string Reply::welcomeMessage()
{
    return ":Server 001 : Welcome to the IRC server!\r\n";
}

std::string Reply::whoStartMessage()
{
    return ":Server 314 : RPL_WHOSPCR\r\n";
}

std::string Reply::whoEndMessage()
{
    return ":Server 315 : RPL_ENDOFWHO\r\n";
}

std::string Reply::whoIsEndMessage()
{
    return ":Server 318 : RPL_ENDOFWHOIS\r\n";
}

std::string Reply::whoMessage(const Client &client)
{
    std::string message = ":Server 311 : " + client.getNickname() 
    + " " + client.getUsername() + " " 
    + client.getHostname() + " " 
    + client.getRealname() + "\r\n" ;
    return message;
}
