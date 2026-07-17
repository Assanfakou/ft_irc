#include "Server.hpp"

#pragma once

class Message;
class Parser
{
    public:
        Message parse(const std::string& line);

    private:
        void parseCommand(Message &msg, const std::string &line, size_t &pos);
        void parseParameters(Message &msg, const std::string &line, size_t &pos);
        std::string toUpper(const std::string &str);
};