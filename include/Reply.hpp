#pragma once

#include <iostream>
#include "Client.hpp"
#include "Message.hpp"

class Client;
class Message;

class Reply
{
    public:
        Reply();
        

        std::string generateMEssage(const Client &client, const Message &msg);

        std::string noTextToSend(const Server &server);
        std::string noSuchNick(const Server &server);
        std::string needMoreParams(const Server &server);
        std::string cantSendToSelf(const Server &server);
        std::string unknownCommand(const Server &server);
        std::string welcomeMessage(const Server &server);
        
        /* who messages */
        std::string whoStartMessage(const Server &server);
        std::string whoEndMessage(const Server &server);
        std::string whoMessage(const Server &server, const Client &client);

        /* WhoIs message */
        std::string whoIsMessage(const Client &client);
        std::string whoIsEndMessage(const Server &server);
};