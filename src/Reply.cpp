#include "../include/Server.hpp"

std::string generateMEssage(const Client &client, const Message &msg)
{
    std::string message = IRC_BLUE + ":" + client.getPrefix() + " " + msg.getCommand() + " :";
    for (size_t i = 0; i < msg.getParams().size(); ++i)
    {
        message += msg.getParameter(i) + " ";
    }
    message += "\r\n" + IRC_RESET;
    return message;
}

std::string noTextToSend(const Server &server)
{
    return IRC_RED + server.getServerName() + " 412 : ERR_NOTEXTTOSEND\r\n" + IRC_RESET;
}

std::string noSuchNick(const Server &server)
{
    return IRC_RED + server.getServerName() + " 401 : ERR_NOSUCHNICK\r\n" + IRC_RESET;
}

std::string needMoreParams(const Server &server)
{
    return IRC_RED + server.getServerName() + " 461 : ERR_NEEDMOREPARAMS\r\n" + IRC_RESET;
}

std::string cantSendToSelf(const Server &server)
{
    return IRC_RED + server.getServerName() + " 666 : ERR_CANTSENDTOSELF\r\n" + IRC_RESET;
}

std::string unknownCommand(const Server &server)
{
    return IRC_RED + server.getServerName() + " 421 : ERR_UNKNOWNCOMMAND\r\n" + IRC_RESET;
}

std::string welcomeMessage(const Server &server)
{
    return IRC_GREEN + ":" + server.getServerName() + " 001 : Welcome to the IRC server!\r\n" + IRC_RESET;
}

std::string whoStartMessage(const Server &server)
{
    return IRC_CYAN + ":" + server.getServerName() + " 314 : RPL_WHOSPCR\r\n" + IRC_RESET;
}

std::string whoEndMessage(const Server &server)
{
    return IRC_CYAN + ":" + server.getServerName() + " 315 : RPL_ENDOFWHO\r\n" + IRC_RESET;
}

std::string whoIsEndMessage(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 318 : RPL_ENDOFWHOIS\r\n" + IRC_RESET;
}

std::string whoMessage(const Server &server,const Client &client)
{
    std::string message = IRC_BLUE + ":" +  server.getServerName() + " 311 : " + client.getNickname() 
    + " " + client.getUsername() + " " 
    + client.getHostname() + " " 
    + client.getRealname() + "\r\n" + IRC_RESET;
    return message;
}

std::string clientNotRegestred(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 451 : ERR_NOTREGISTERED\r\n" + IRC_RESET;
}

std::string passwordAccepted(const Server &server)
{
    return IRC_BROWN + ":" + server.getServerName() + " Password Accepted\r\n" + IRC_RESET;
}