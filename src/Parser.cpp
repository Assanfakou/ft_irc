#include "../include/Parser.hpp"

Message Parser::parse(const std::string& line)
{
    Message msg;
    size_t i = 0;
    
    while (i < line.length())
    {
       if (line[i] == ' ')
       {
            i++;
            continue;
       }
       else
       {
           parseCommand(msg, line, i);
           parseParameters(msg, line, i);
           break;
       }
    }
    return msg;
}
void Parser::parseParameters(Message &msg, const std::string &line, size_t &i)
{
    std::string parameter;

    while(i < line.length())
    {
        if (line[i] == ':')
        {
            msg.pushBackParam(line.substr(i + 1, line.length()));
            break;
        }
        while(i < line.length() && line[i] != ' ')
        {
            parameter += line[i];
            i++;
        }
        if (!parameter.empty())
        {
            msg.pushBackParam(parameter);
            parameter.clear();
        }
        i++;
    }
}

std::string Parser::toUpper(const std::string &str)
{
    std::string upperStr;
    for (size_t i = 0; i < str.length(); ++i)
        upperStr += std::toupper(str[i]);
    return upperStr;
}

void Parser::parseCommand(Message &msg, const std::string &line, size_t &i)
{
    std::string command;

    while (i < line.length() && line[i] != ' ')
    {
        command += line[i];
        i++;
    }
    i++;
    msg.setCommand(toUpper(command));
}