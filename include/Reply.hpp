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

        std::string noTextToSend(const Client &client);
        std::string noSuchNick(const Client &client);
        std::string needMoreParams(const Client &client);
        std::string cantSendToSelf(const Client &client);
        std::string unknownCommand(const Client &client);
        std::string welcomeMessage(const Client &client);
};