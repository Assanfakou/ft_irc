#include "../../include/Server.hpp"

void connectToServer(int socket, std::string password)
{
    std::string sendT = "pass " + password + "\r\n";
    send(socket, sendT.c_str(), sendT.size(), 0);
    sendT = "nick l7aj\r\n";
    send(socket, sendT.c_str(), sendT.size(), 0);
    sendT = "user l7aj 0 * : l7aj boujm3a\r\n";
    send(socket, sendT.c_str(), sendT.size(), 0);
    
    sendT = "join #l7aj\r\n";
    send(socket, sendT.c_str(), sendT.size(), 0);
    return ;
}

void botJoinCHannel(int socketfd, const std::string &channelName)
{
    std::string msg = "join " + channelName + "\r\n";
    send(socketfd, msg.c_str(), msg.size(), 0);
}

void parseBotMessage(Message &msg, int socketfd)
{
    std::cout << IRC_GREEN << msg.getParameter(1) << IRC_RESET;
    if (msg.getParameter(0) == "INVITE")
    {
        botJoinCHannel(socketfd, msg.getParameter(2));
        return ;
    }
    if (msg.getParameter(2) == "!help\r\n")
    {
        std::cout << "here \n";
        help(socketfd);
        return ;
    }
    if (msg.getParameter(2) == "!coin\r\n") 
    {
       sendBot(socketfd, botMessage("This is your Shot ; " + coin()));
       return ;
    }
    if (msg.getParameter(2) == "!roll\r\n")
    {
        sendBot(socketfd, botMessage("This is your shot : " + convertintToString(roll())));
        return ;
    }
    if (msg.getParameter(2) == "!time\r\n")
    {
        sendBot(socketfd, botMessage(sendCurrentTime()));
        return ;
    }
    if (msg.getParameter(2) == "!quote\r\n")
    {
        sendBot(socketfd, botMessage("Quote : " + quote()));
        return ;
    }
    if (msg.getParameter(2) == "!fact\r\n")
    {
        sendBot(socketfd, botMessage("Fact : " + fact()));
        return ;
    }
    if (msg.getParameter(2) == "!joke\r\n")
    {
        sendBot(socketfd,botMessage("Joke : " + joke()));
        return ;
    }
    return ;
}

int main(int ac, char **av)
{
    (void) ac;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << IRC_RED << "socket" << IRC_RESET;
        return 1;
    }
    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(std::atoi(av[2]));
    inet_pton(AF_INET, av[1], &server.sin_addr);

    if (connect(sock, (sockaddr *)&server, sizeof(server)) == -1)
    {
        std::cerr << IRC_RED << "connect" << IRC_RESET;
        return 1;
    }
    connectToServer(sock, av[3]);
    while (true)
    {
        char buff[1024];
        int bytes = recv(sock, buff, sizeof(buff) - 1, 0);
        if (bytes <= 0)
        {
            std::cerr << IRC_RED << "Disconnected\n" << IRC_RESET;
            close(sock);
            return 1;
        }
        buff[bytes] = '\0';
        Parser parser;
        Message msg = parser.parse(buff); 
        parseBotMessage(msg, sock);
        std::cout << msg.getCommand() << " " << msg.getParameter(0) << " " << msg.getParameter(1) << " " << msg.getParameter(2) << "\n";
    }
}