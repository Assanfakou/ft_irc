#include "../../include/Message.hpp"
#include "../../include/Reply.hpp"

void privmsg(Server &server, Client &client, const Message &msg);
void notice(Server &server, Client &client, const Message &msg);