#include "commands.hpp"

void privmsg(Server &server, Client &sender, const Message &msg)
{
    (void) sender;  
    std::string reciever = msg.getParameter(0);
    if (msg.getParams().size() < 2)
    {
        if (server.getClientByNickname(msg.getParameter(0)))
        {
            std::string errorMessage = ":server 400 : No text to send\r\n";
            server.sendMessageToClient(sender.getFd(), errorMessage);
            return ;
        }
        else
        {
            std::string errorMessage = ":Server 401 " + sender.getNickname() + " " + msg.getParameter(0) + ": No nick/chanel\r\n";
            server.sendMessageToClient(sender.getFd(), errorMessage);
            return ;
        }
    }
    std::string message = msg.getParameter(1);
    Client *receiverClient = server.getClientByNickname(reciever);
    if (receiverClient)
    { 
        std::string fullMessage = ":" + sender.getPrefix() + " PRIVMSG " + reciever + " :" + message + "\r\n";
        server.sendMessageToClient(receiverClient->getFd(), fullMessage);
        std::cout << Blue << "Message sent to " << reciever << ": " << message << RESET;
        return ;
    }
    else
    {
            std::string errorMessage = ":server 401 : No Reciver\r\n";
            server.sendMessageToClient(sender.getFd(), errorMessage);
            return ;
    }
    std::cout << Blue << "Everything is good\n" << RESET;
}