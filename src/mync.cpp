#include "../include/Server.hpp"

int main(int ac , char **av)
{
    (void) ac;
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd < 0)
    {
        std::cerr << "this socketfd is not succeed\n";
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(std::atoi(av[2]));

    if (inet_pton(AF_INET, av[1], &server.sin_addr) <= 0)
    {
        std::cerr << "Invalid Ip addresses\n";
        return 1;
    }

    if (connect(socketfd, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        std::cerr << "Connection failed\n";
        return 1;
    }
   while (1)
   {
        std::string msg;
        std::getline(std::cin, msg);
        msg += "\r\n";
        send(socketfd, msg.c_str(), msg.length(), 0);
   } 
}