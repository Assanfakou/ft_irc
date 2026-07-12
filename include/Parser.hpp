#pragma once

#include "msg.hpp"


class Parse
{
    public:
        Message parse(const std::string& line);
        void setCommand(Message &msg, const std::string &command);
        void setParameters(Message &msg, const std::vector<std::string> &params);

    private:
        void parseCommand(Message &msg, const std::string &line, size_t &pos);
        void parseParameters(Message &msg, const std::string &line, size_t &pos);
        std::string toUpper(const std::string &str);
};