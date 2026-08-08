#include "Server.hpp"

std::string convertintToString(int num);
bool compareNick(const std::string &a, const std::string &b);
std::string convertintToString(int num);
std::string help();
void sendBot(int socketfd, const std::string &msg);
std::string coin();
std::string sendCurrentTime();
int roll();
std::string joke();
std::string fact();
std::string quote();
void sendToServer(int socketfd, std::string &nickname, std::string &command);