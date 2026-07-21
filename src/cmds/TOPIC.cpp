#include "../../include/Server.hpp"

void Server::showTopic(const Message &msg, Client &client)
{

    std::map<std::string, Channel>::iterator it = _channels.find(msg.getParameter(0));

    if (it == _channels.end())
    {
        std::cout << "Channel not found !!" << std::endl;
        return;
    }

    if (!msg.getParameter(1).empty())
    {
        if (it->second.isTopicRestricted())
        {
            std::cout << "You don't have permission to change the topic of this channel." << std::endl;
            return;
        }
        if (it->second.isOperator(client.getFd()))
        {
            it->second.setTopic(msg.getParameter(1));
            std::cout << "Topic set to: "
                      << msg.getParameter(1)
                      << " "
                      << it->second.getTopic()
                      << std::endl;
        }
    }
    else
    {
        std::cout << "Topic of "
                  << msg.getParameter(0)
                  << ": "
                  << it->second.getTopic()
                  << std::endl;
    }
}
