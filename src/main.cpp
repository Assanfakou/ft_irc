#include "../include/Server.hpp"
#include <iostream>
#include <cstdlib>

int port_parse(std::string port)
{
    size_t check = port.find_first_not_of("0123456789");
    if (check == std::string::npos)
    {
        int portNum = std::atoi(port.c_str());
        if (portNum >= 1024 && portNum <= 65535)
            return portNum;
        else
            return -1;
    }
    return -1;
}

int main(int ac, char **av)
{
    int port;
    if (ac != 3)
    {
        std::cerr << IRC_RED << "Usage: ./ircserv <port> <password>" << std::endl << IRC_RESET;
        return 1;
    }
    port = port_parse(av[1]);
    if (port == -1)
    {
        std::cerr << IRC_RED << "Port Number Range between 1024-65535" << '\n' << IRC_RESET;
        return -1;
    }
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
