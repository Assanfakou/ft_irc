#include "../include/Parser.hpp"

Message Parser::parse(const std::string& line)
{
    Message msg;
    size_t i = 0;
    
    while (i < line.length())
    {
        
    }
    
}

void Parser::parseCommand(Message &msg, const std::string &line, size_t &i)
{
    std::string command;

    while (i < line.size() && line[i] != ' ')
    {
        command += line[i];
        i++;
    }
    msg.setCommand(toUpper(command));
}

void Parser::setCommand(Message &msg, const std::string &command)
{
    
}
void setParameters(Message &msg, const std::vector<std::string> &params);