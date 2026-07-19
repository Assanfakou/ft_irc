#include "../../include/Server.hpp"
/*
**
** The list message is used to list channels and their topics.  If  the
** <channel>  parameter  is  used,  only  the  status  of  that  channel
** is displayed.  Private  channels  are  listed  (without  their
** topics)  as channel "Prv" unless the client generating the query is
** actually on that channel.  Likewise, secret channels are not listed
**
*/

std::string convertintToString(int num)
{
    // static std::string number;
    // while (num > 9)
    // {
    //     number += num % 10 + '0';
    //     num /= 10;
    // }
    // number += num % 10 + '0';
    // if (num < 10)
    //     return number;
    // else 
    // {
    //     int begin = 0;
    //     int end = number.length() - 1;

    //     while (begin < end)
    //     {
    //         std::swap(number[begin], number[end]);
    //         end--;
    //         begin++;
    //     }
    //     return number;
    // }
    std::stringstream s;
    s << num;
    return s.str();
}

void Server::listChanels(const Client &client, const Message &msg)
{
    if (!client.hasPassAccepted() && !client.isRegistered())
    {
        sendMessageToClient(client.getFd(), clientNotRegestred(*this));
        return ;
    }
    std::map<std::string, Channel>::iterator it = _channels.begin();
    std::string chanNames;
    for (; it != _channels.end(); ++it)
        chanNames += it->first + ": " + convertintToString(it->second.getMembers().size()) + " :" + it->second.getTopic() + "\r\n";
    std::cout << "chanelels : " << chanNames + "\r\n";
    sendMessageToClient(client.getFd(), startMessage(*this, msg));
    sendMessageToClient(client.getFd(), chanNames);
    sendMessageToClient(client.getFd(), EndMessage(*this, msg));
}