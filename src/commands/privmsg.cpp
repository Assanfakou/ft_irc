#include "commands.hpp"

void privmsg(Server &server, Client &sender, const Message &msg)
{
    (void) sender;  
    if (msg.getParams().size() < 2)
    {
        if (server.getClientByNickname(msg.getParameter(0)))
        {
            std::cout << RED << "no message to send\n" << RESET;
            return ;
        }
        else
        {
            std::cout << RED << "no Reciever mentioned \n" << RESET;
            return ;
        }
    }
    std::string reciever = msg.getParameter(0);
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
        std::cout << RED << "Reciever not found\n" << RESET;
        return ;
    }
    std::cout << Blue << "Everything is good\n" << RESET;
}