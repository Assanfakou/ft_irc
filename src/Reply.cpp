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
    return IRC_RED + ":" + server.getServerName() + " 412 : ERR_NOTEXTTOSEND\r\n" + IRC_RESET;
}

std::string noSuchNick(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 401 : ERR_NOSUCHNICK\r\n" + IRC_RESET;
}

std::string needMoreParams(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 461 : ERR_NEEDMOREPARAMS\r\n" + IRC_RESET;
}

std::string cantSendToSelf(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 666 : ERR_CANTSENDTOSELF\r\n" + IRC_RESET;
}

std::string unknownCommand(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " 421 : ERR_UNKNOWNCOMMAND\r\n" + IRC_RESET;
}

std::string welcomeMessage(const Server &server)
{
    return IRC_GREEN + ":" + server.getServerName() + " 001 : Welcome to the IRC server!\r\n" + IRC_RESET;
}

std::string startMessage(const Server &server, const Message msg)
{
    return IRC_GREEN + ":" + server.getServerName() + msg.getCommand() + " RPL_START \r\n";
}

std::string EndMessage(const Server &server, const Message msg)
{
    return ":" + server.getServerName() + msg.getCommand() + " RPL_END \r\n" + IRC_RESET;
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
std::string pong(const Server &server, const Message &msg)
{
    return IRC_BLUE + ":" + server.getServerName() + " PONG :" + msg.getParameter(0) + "\r\n";
}
std::string wrongPassword(const Server &server)
{
    return IRC_RED + ":" + server.getServerName() + "  Wrong Password\r\n" + IRC_RESET;
}

std::string joinChannel(const Server &server, const Client &sender, const std::string &chanelName)
{
    (void) server;
    return ":" + sender.getPrefix() + " JOIN " + chanelName + "\r\n";
}
std::string topicWhenJoin(const Server &server, const Client &client, const Channel &channel)
{
    std::string topic;

    if (channel.getTopic().empty())
    {
        topic = "No Topic is set";
        return ":" + server.getServerName() + " 331 " + client.getNickname() + " " + channel.getName() + " :" + topic + "\r\n";
    }
    else
        topic = channel.getTopic();
    return ":" + server.getServerName() + " 332 " + client.getNickname() + " "
    + channel.getName() + " :" + topic + "\r\n"; 
}
   
//RPL_NAMREPLY
std::string namesWhenJoin(Server &server, const Client &client, const Channel &channel)
{
    return ":" + server.getServerName() + " 353 " + client.getNickname() + " = " + channel.getName() + " : " + server.getChanelUsers(channel.getName()) 
    + "\r\n" + endOfNamesList(server, client, channel);
}

std::string endOfNamesList(Server &server, const Client &client, const Channel &channel)
{
    return ":" + server.getServerName() + " 366 " + client.getNickname()
    + " " + channel.getName() + " :End of /Names list\r\n";
}
std::string notValidChanelName(Server &server)
{
    return IRC_RED + ":" + server.getServerName() + " This is not a valid name for channel Name \r\n" + IRC_RESET;
}