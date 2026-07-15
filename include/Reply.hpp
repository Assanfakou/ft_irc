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

        std::string noTextToSend();
        std::string noSuchNick();
        std::string needMoreParams();
        std::string cantSendToSelf();
        std::string unknownCommand();
        std::string welcomeMessage();
        
        /* who messages */
        std::string whoStartMessage();
        std::string whoEndMessage();
        std::string whoMessage(const Client &client);

};