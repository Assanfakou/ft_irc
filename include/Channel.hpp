#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <iostream>

class Channel
{
    private:
        std::string _name;
        std::vector<int> _members;
        std::vector<int> _invitedClients;
        std::string _topic;
        bool _inviteOnly;
        bool _topicRestricted;
        std::vector<int> _operators;

    public:
        Channel(const std::string &name);
        const std::string &getName() const;
        void addMember(int fd);
        bool isMember(int fd);
        void leaveChannel(std::string channelName, int fd);
        void addInvitedClient(int fd);
        bool isInvited(int fd);
        void setTopic(const std::string &topic);
        std::string getTopic() const;
        void setInviteOnly(bool value);
        bool isInviteOnly() const;
        void setTopicRestricted(bool value);
        bool isTopicRestricted() const;
        void addOperator(int fd);
        void removeOperator(int fd);
        bool isOperator(int fd);
        std::vector<int> &getMembers();

};

#endif