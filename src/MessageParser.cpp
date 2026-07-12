#include "../include/MessageParser.hpp"

MessageParser::MessageParser() : prefix(""), command(""), params() {}

void MessageParser::parseMessage(const std::string &line)
{
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
            parseCommand(line, i);
            parseParams(line, i);
            break;
        }
    }
}
void MessageParser::printMessage() const
{
    std::cout << Blue << "Command -> " << RED << command << "\n"
              << RESET;
    std::cout << Blue << "Parameter size ->> " << RED << params.size() << "\n"
              << RESET;
    for (size_t i = 0; i < params.size(); i++)
        std::cout << Blue << "Param[" << i << "] ->> " << RED << params[i] << "\n"
                  << RESET;
}

std::string MessageParser::toUpper(const std::string& command)
{
    std::string Uppercomand;
    for (size_t i = 0; i < command.length(); i++)
        Uppercomand += std::toupper(command[i]);
    return Uppercomand;
}

void MessageParser::parseCommand(const std::string &line, size_t &i)
{
    std::string command;

    while (i < line.length() && line[i] != ' ')
    {
        command += line[i];
        i++;
    }
    i++;
    setCommand(toUpper(command));

}
void MessageParser::parseParams(const std::string &line, size_t &i)
{
    std::string parameter;
    std::vector<std::string> params;

    while (i < line.length())
    {
        if (line[i] == ':')
        {
            params.push_back(line.substr(i + 1, line.length()));
            break;
        }
        while (i < line.length() && line[i] != ' ')
        {
            parameter += line[i];
            i++;
        }
        if (!parameter.empty())
        {
            params.push_back(parameter);
            parameter.clear();
        }
        i++;
    }
    setParams(params);
}

const std::string &MessageParser::getPrefix() const
{
    return prefix;
}
const std::string &MessageParser::getCommand() const
{
    return command;
}
const std::vector<std::string> &MessageParser::getParams() const
{
    return params;
}

void MessageParser::setPrefix(const std::string &p)
{
    prefix = p;
}
void MessageParser::setCommand(const std::string &c)
{
    command = c;
}
void MessageParser::MessageParser::setParams(const std::vector<std::string> &p)
{
    params = p;
}
