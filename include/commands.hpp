#include "Message.hpp"
#include "Reply.hpp"

void privmsg(Server &server, Client &client, const Message &msg);
void notice(Server &server, Client &client, const Message &msg);
void who(Server &server, Client &sender, const Message &msg);
void nickHandler(Client &client, Server &server, const std::string nickName);
void passHandler(Server &server, Client &client, const std::string &password);
void userHandler(Server &server, Client &client, const Message &msg);