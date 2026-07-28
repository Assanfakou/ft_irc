#include "../include/Server.hpp"

Message::Message() : prefix(""), command(""), params() {}

void Message::printMessage() const
{
    std::cout << "Command -> " << command << "\n";
    std::cout << "Parameter size ->> " << params.size() << "\n";
    for (size_t i = 0; i < params.size(); i++)
        std::cout << "Param[" << i << "] ->> " << params[i] << "\n";
}

const std::string &Message::getPrefix() const
{
    return prefix;
}
const std::string &Message::getCommand() const
{
    return command;
}
const std::vector<std::string> &Message::getParams() const
{
    return params;
}

const std::string &Message::getParameter(size_t index) const
{
    static const std::string chew;
    if (index < params.size())
        return params.at(index);
    else
       return chew;
}

void Message::setPrefix(const std::string &p)
{
    prefix = p;
}
void Message::setCommand(const std::string &c)
{
    command = c;
}
void Message::setParams(const std::vector<std::string> &p)
{
    params = p;
}
void Message::pushBackParam(const std::string &param)
{
    params.push_back(param);
}
