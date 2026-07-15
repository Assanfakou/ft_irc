#include "../../include/Message.hpp"
#include "../../include/Reply.hpp"

void privmsg(Server &server, Client &client, const Message &msg);
void notice(Server &server, Client &client, const Message &msg);
void whoIs(Server &server, Client &sender, const Message &msg);
void who(Server &server, Client &sender, const Message &msg);