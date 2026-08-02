#include "../include/Server.hpp"
#include <iostream>
#include <cstdlib>

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << IRC_RED << "Usage: ./ircserv <port> <password>" << std::endl << IRC_RESET;
        return 1;
    }
    int i = 0;
    while (av[1][i])
    {
        if (std::isdigit(av[1][i]))
            i++;
        else
        {
            std::cout << IRC_RED << "The port should be a Number\n" << IRC_RESET;
            return 1;
        }
    }
    int port = std::atoi(av[1]);
    std::string password = av[2];
    try
    {
        signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
        Server server(port, password);
        server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << IRC_RED << e.what() << '\n' << IRC_RESET;
        return 1;
    }
    return 0;
}

