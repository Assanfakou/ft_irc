#include "../include/Message.hpp"

Message::Message() : prefix(""), command(""), params() {}

void Message::printMessage() const
{
    std::cout << Blue << "Command -> " << RED << command << "\n"
              << RESET;
    std::cout << Blue << "Parameter size ->> " << RED << params.size() << "\n"
              << RESET;
    for (size_t i = 0; i < params.size(); i++)
        std::cout << Blue << "Param[" << i << "] ->> " << RED << params[i] << "\n"
                  << RESET;
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
