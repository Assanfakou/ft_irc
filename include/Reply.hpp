#pragma once

#include <iostream>

class Client;
class Message;
class Server;

std::string generateMEssage(const Client &client, const Message &msg);

std::string noTextToSend(const Server &server);
std::string noSuchNick(const Server &server, const Client &client, const std::string &targetName);
std::string needMoreParams(const Server &server, const Client &client, const Message &msg);
std::string cantSendToSelf(const Server &server);
std::string unknownCommand(const Server &server);
std::string welcomeMessage(const Server &server, const Client &client);

/* who messages */
std::string whoStartMessage(const Server &server);
std::string whoEndMessage(const Server &server);
std::string whoMessage(const Server &server, const Client &client);

/* WhoIs message */
std::string whoIsMessage(const Client &client);
std::string whoIsEndMessage(const Server &server);
std::string unknownCommand(const Server &server);


std::string clientNotRegestred(const Server &server);
std::string passwordAccepted(const Server &server);
std::string pong(const Server &Server, const Message &msg);
std::string startMessage(const Server &server, const Message msg);
std::string EndMessage(const Server &server, const Message msg);


std::string joinChannel(const Server &server, const Client &sender, const std::string &chanelName);
std::string topicWhenJoin(const Server &server, const Client &client, const Channel &channel);
std::string endOfNamesList(Server &server, const Client &client, const Channel &channel);
std::string namesWhenJoin(Server &server, const Client &client, const Channel &channel);
std::string notValidChanelName(Server &server);


std::string kickMessage(const Client &sender, const Message &msg);
std::string chanOpPrivsNeeded(const Server &server, const Message &msg);
std::string userNotInChannel(const Server &server, const Client &client, const Message &msg);
std::string notOnChannel(const Server &server, const Client &client, const Message &msg);
std::string noSuchChannel(const Server &server, const Client &client , const Message &msg);

/*invite*/

std::string inviteMessage(const Client &sender, const Message &msg);
std::string inviting(const Server &server, const Client &client, const Message &msg);
std::string userOnChannel(const Server &server, const Client &client, const Message &msg);
