#include "../include/Channel.hpp"

Channel::Channel(const std::string &name): _name(name), _inviteOnly(false), 
    _topicRestricted(false)
{

}
void Channel::addMember(int fd)
{
    _members.push_back(fd);
}
bool Channel::isMember(int fd)
{
    std::vector<int>::iterator it;
    for (it = _members.begin(); it != _members.end(); it++)
    {
        if (*it == fd)
            return (true);
    }
    return (false);
}

void Channel::addInvitedClient(int fd)
{
    _invitedClients.push_back(fd);
}

bool Channel::isInvited(int fd)
{
    std::vector<int>::iterator it;
    for (it = _invitedClients.begin(); it != _invitedClients.end(); it++)
    {
        if (*it == fd)
            return (true);
    }
    return (false);
}

void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}

std::string Channel::getTopic() const
{
    return _topic;
}

void Channel::setInviteOnly(bool value)
{
    _inviteOnly = value;
}

bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}

void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}

bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

void Channel::addOperator(int fd)
{
    _operators.push_back(fd);
}

void Channel::removeOperator(int fd)
{
    std::vector<int>::iterator it;
    for (it = _operators.begin() ; it != _operators.end(); it++)
    {
        if (*it == fd)
        {
            _operators.erase(it);
            break;
        }
    }
}

bool Channel::isOperator(int fd)
{
    std::vector<int>::iterator it;
    for (it = _operators.begin() ; it != _operators.end(); it++)
    {
        if (*it == fd)
            return true;
    }
    return false;
}

std::vector<int> &Channel::getMembers()
{
    return _members;
}

const std::string &Channel::getName() const
{
    return _name;
}