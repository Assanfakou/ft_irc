#include "Server.hpp"

void privmsg(Server &server, Client &client, const Message &msg);
void notice(Server &server, Client &client, const Message &msg);
void who(Server &server, Client &sender, const Message &msg);
void nickHandler(Client &client, Server &server, const Message &msg);
void passHandler(Server &server, Client &client, const Message &msg);
void userHandler(Server &server, Client &client, const Message &msg);