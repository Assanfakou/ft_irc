#include "../include/Server.hpp"

std::string generateMEssage(const Client &client, const Message &msg)
{
    std::string message;

    message = ":" + client.getPrefix() + " ";
    message += msg.getCommand();
    message += " ";
    message += msg.getParameter(0);      // target
    message += " :";
    message += msg.getParameter(1);      // text
    message += "\r\n";

    return message;
}


std::string noTextToSend(const Server &server)
{
    return ":" + server.getServerName() + " 412 : ERR_NOTEXTTOSEND\r\n";
}

std::string noSuchNick(const Server &server, const Client &client, const std::string &targetName)
{
    return ":" + server.getServerName() + " 401 " + client.getNickname() + " " + targetName + " :No suchNick\r\n";
}

std::string noSuchCnannel(const Server &server, const Client &client, const std::string &targetName)
{
    return ":" + server.getServerName() + " 403 " + client.getNickname() + " " + targetName + " :No such Channel\r\n";
}

std::string needMoreParams(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName() + " 461 " + client.getNickname() + " " + msg.getCommand() + " :Not enough parameters\r\n";
}

std::string cantSendToSelf(const Server &server)
{
    return ":" + server.getServerName() + " 666 : ERR_CANTSENDTOSELF\r\n";
}

std::string unknownCommand(const Server &server)
{
    return  ":" + server.getServerName() + " 421 : ERR_UNKNOWNCOMMAND\r\n";
}

std::string welcomeMessage(const Server &server, const Client &client)
{
    return ":" + server.getServerName() + " 001 " + client.getNickname() + " :Welcome to the IRC server!\r\n";
}

std::string noMotd(Server &server, Client &client)
{
    return ":" + server.getServerName() +
           " 422 " + client.getNickname() +
           " :MOTD File is missing\r\n";
}

std::string startMessage(const Server &server, const Message msg)
{
    return ":" + server.getServerName() + " " + msg.getCommand() + " RPL_START \r\n";
}

std::string EndMessage(const Server &server, const Message msg)
{
    return ":" + server.getServerName() + " " + msg.getCommand() + " RPL_END \r\n";
}

std::string whoStartMessage(const Server &server)
{
    return ":" + server.getServerName() + " 314 \r\n";
}

std::string whoEndMessage(const Server &server)
{
    return ":" + server.getServerName() + " 315 \r\n";
}

std::string whoMessage(const Server &server,const Client &client)
{
    std::string message = ":" +  server.getServerName() + " 311 : " + client.getNickname() 
    + " " + client.getUsername() + " " 
    + client.getHostname() + " " 
    + client.getRealname() + "\r\n";
    return message;
}

std::string clientNotRegestred(const Server &server)
{
    return  ":" + server.getServerName() + " 451 : Not regestred\r\n";
}

std::string passwordAccepted(const Server &server)
{
    return ":" + server.getServerName() + " Password Accepted\r\n";
}
std::string pong(const Server &server, const Message &msg)
{
    return ":" + server.getServerName() + " PONG :" + msg.getParameter(0) + "\r\n";
}
std::string wrongPassword(const Server &server)
{
    return  ":" + server.getServerName() + " 464 * :Wrong Password\r\n";
}

std::string joinChannel(const Server &server, const Client &sender, const std::string &chanelName)
{
    (void) server;
    //return ":" + sender.getPrefix() + " JOIN :" + chanelName + "\r\n";
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
    + channel.getName() + " : " + topic + "\r\n"; 
}
   
//RPL_NAMREPLY
std::string namesWhenJoin(Server &server, const Client &client, const Channel &channel)
{
    return ":" + server.getServerName() + " 353 " + client.getNickname() + " = " + channel.getName() + " :" + server.getChanelUsers(channel.getName()) 
    + "\r\n";
}

std::string endOfNamesList(Server &server, const Client &client, const Channel &channel)
{
    return ":" + server.getServerName() + " 366 " + client.getNickname()
    + " " + channel.getName() + " :End of /NAMES list\r\n";
}
std::string notValidChanelName(Server &server)
{
    return  ":" + server.getServerName() + " This is not a valid name for channel Name \r\n";
}

// Broadcast when a user is kicked.
std::string kickMessage(const Client &sender, const Message &msg)
{
    std::string mssg = ":" + sender.getPrefix() + " KICK " + msg.getParameter(0) + " " + msg.getParameter(1);
    if (!msg.getParameter(2).empty())
        mssg += " :" + msg.getParameter(2);

    mssg += "\r\n";
    return mssg;
}
std::string chanOpPrivsNeeded(const Server &server, const Message &msg)
{
    return ":" + server.getServerName()
        + " 482 "
        + msg.getParameter(1)
        + " "
        + msg.getParameter(0)
        + " :You're not channel operator\r\n";
}

std::string userNotInChannel(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 441 "
        + client.getNickname()
        + " "
        + msg.getParameter(1)
        + " "
        + msg.getParameter(0)
        + " :Doesn't exist on that channel\r\n";
}
std::string notOnChannel(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 442 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :You're not on that channel\r\n";
}
std::string noSuchChannel(const Server &server, const Client &client , const Message &msg)
{
    return ":" + server.getServerName()
        + " 403 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :No such channel\r\n";
}

// ERR_USERONCHANNEL (443)
std::string userOnChannel(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 443 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " "
        + msg.getParameter(1)
        + " :is already on channel\r\n";
}

// RPL_INVITING (341)
std::string inviting(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 341 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " "
        + msg.getParameter(1)
        + "\r\n";
}

// INVITE message sent to the invited user
std::string inviteMessage(const Client &sender, const Message &msg)
{
    return ":" + sender.getPrefix()
        + " INVITE "
        + msg.getParameter(0)
        + " :"
        + msg.getParameter(1)
        + "\r\n";
}
// ERR_CHANNELISFULL (471)
std::string channelIsFull(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 471 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :Cannot join channel (+l)\r\n";
}
// ERR_USERONCHANNEL (443)
std::string userAlreadyOnChannel(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 443 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :is already on channel\r\n";
}
// ERR_BADCHANNELKEY (475)
std::string badChannelKey(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 475 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :Cannot join channel (+k)\r\n";
}
// ERR_INVITEONLYCHAN (473)
std::string inviteOnlyChan(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 473 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :Cannot join channel (+i)\r\n";
}

std::string modeMessage(const Client &sender, const Message &msg)
{
    std::string reply = ":" + sender.getPrefix()
        + " MODE "
        + msg.getParameter(0)
        + " "
        + msg.getParameter(1);

    if (!msg.getParameter(2).empty())
        reply += " " + msg.getParameter(2);

    reply += "\r\n";
    return reply;
}
// ERR_INVALIDMODEPARAM (696)
// Used when +l receives an invalid limit.
std::string invalidModeParameter(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 696 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " "
        + msg.getParameter(1)
        + " :Invalid mode parameter\r\n";
}
// ERR_UNKNOWNMODE (472)
std::string unknownMode(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 472 "
        + client.getNickname()
        + " "
        + msg.getParameter(1)
        + " :is unknown mode char to me\r\n";
}

std::string partMessage(const Client &client, const Message &msg)
{
    std::string reply = ":" + client.getPrefix()
        + " PART "
        + msg.getParameter(0);

    if (!msg.getParameter(1).empty())
        reply += " :" + msg.getParameter(1);

    reply += "\r\n";
    return reply;
}

std::string topicMessage(const Client &client, const Message &msg)
{
    return ":" + client.getPrefix()
        + " TOPIC "
        + msg.getParameter(0)
        + " :"
        + msg.getParameter(1)
        + "\r\n";
}
std::string noTopic(const Server &server, const Client &client, const Message &msg)
{
    return ":" + server.getServerName()
        + " 331 "
        + client.getNickname()
        + " "
        + msg.getParameter(0)
        + " :No topic is set\r\n";
}

std::string topicReply(const Server &server, const Client &client, const Channel &channel)
{
    return ":" + server.getServerName()
        + " 332 "
        + client.getNickname()
        + " "
        + channel.getName()
        + " :"
        + channel.getTopic()
        + "\r\n";
}
std::string botMessage(const Client &client, const std::string &text)
{
    return ":l7aj!bot@localhost PRIVMSG "
        + client.getNickname()
        + " :"
        + text
        + "\r\n";
}
std::string entringMessage(Server &server)
{
    std::string notice;

    notice += ":" + server.getServerName() + " NOTICE * :--------------------------------------------------\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :        Welcome to the Aliens IRC Server!\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :--------------------------------------------------\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :To begin, please register using:\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  pass <password>\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  nick <nickname>\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  user <username> 0 * :<realname>\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :\r\n";
    return notice;
}

std::string serverNotice(Server &server)
{
    std::string notice;
    notice += ":" + server.getServerName() + " NOTICE * :  These commands you can use:\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /join #channel               Join or create a channel\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /list                        List all channels\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /privmsg <nick> :<msg>       Send a private message\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /privmsg #channel :<msg>     Send a message to a channel\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /part #channel               Leave a channel\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /invite <nick> #channel      Invite a user\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /mode #chan ...              Manage channel modes\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /topic #chan <topic>         Change or view the topic\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /kick <nick> #chan           Kick a user (operator only)\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :  /who #channel                List channel users\r\n";

    notice += ":" + server.getServerName() + " NOTICE * :Channel modes:\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :+i Invite Only\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :+t Topic by Operators Only\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :+k Channel Password\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :+l User Limit\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :+o Give Operator Status\r\n";

    notice += ":" + server.getServerName() + " NOTICE * : /quit               Disconnect from the server\r\n";

    notice += ":" + server.getServerName() + " NOTICE * :\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :Enjoy your stay!\r\n";
    notice += ":" + server.getServerName() + " NOTICE * :--------------------------------------------------\r\n";

    return notice;
}