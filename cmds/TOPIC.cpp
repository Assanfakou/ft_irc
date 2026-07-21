#include "../include/Server.hpp"

Topicinfo Server::getTopicInfo(std::string command)
{
    Topicinfo info;

    if (command.size() >= 5 && command.substr(0, 5) == "TOPIC")
    {
        if (command.size() <= 6)
            return info;
        std::string value = command.substr(6);
        for (size_t i = 0; i < value.size(); i++)
        {
            if (value[i] == ' ')
            {
                info.channel = value.substr(0, i);
                info.topic = value.substr(i + 1);
                return info;
            }
        }
        // No topic provided
        info.channel = value;
    }
    return info;
}


void Server::showTopic(Topicinfo topic_info, Client &client)
{
    std::map<std::string, Channel>::iterator it = _channels.find(topic_info.channel);

    if (it == _channels.end())
    {
        std::cout << "Channel not found !!" << std::endl;
        return;
    }

    if (!topic_info.topic.empty())
    {
        if (it->second.isTopicRestricted())
        {
            std::cout << "You don't have permission to change the topic of this channel." << std::endl;
            return;
        }
        if (it->second.isOperator(client.getFd()))
        {
            it->second.setTopic(topic_info.topic);
            std::cout << "Topic set to: "
                      << topic_info.channel
                      << " "
                      << it->second.getTopic()
                      << std::endl;
        }
    }
    else
    {
        std::cout << "Topic of "
                  << topic_info.channel
                  << ": "
                  << it->second.getTopic()
                  << std::endl;
    }
}
