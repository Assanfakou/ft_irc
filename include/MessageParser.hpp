#pragma once

#include "Server.hpp"

class MessageParser
{
    private:
        std::string prefix;
        std::string command;
        std::vector<std::string> params;
    public:
        MessageParser();
        void parseMessage(const std::string &line);

        const std::string &getPrefix() const;
        const std::string &getCommand() const ;
        const std::vector<std::string> &getParams() const;

        void setPrefix(const std::string &p);
        void setCommand(const std::string &c);
        void setParams(const std::vector<std::string> &p);
        void printMessage() const;
    private:
        void parseParams(const std::string &line, size_t &pos);
        void parseCommand(const std::string &line, size_t &pos);
        std::string toUpper(const std::string& command);
};
