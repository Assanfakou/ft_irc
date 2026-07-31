#include "../include/Server.hpp"
#include <ctime>
#include <sstream>

void sendCurrentTime(Server &server, const Client &client)
{
    time_t now = time(NULL);

    tm *local = localtime(&now);
    std::ostringstream oss;
    oss << "Current time: "
        << local->tm_hour << ":"
        << local->tm_min << ":"
        << local->tm_sec;
    server.sendMessageToClient(client.getFd(), botMessage(client, oss.str()));
}

std::string coin()
{
    int coin = rand() % 2;

    if (coin == 0)
        return "head\r\n";
    else
        return "tails\r\n";
}

int roll()
{
    int num = rand() % 6;
    if (num == 0)
        return 6;
    return num;
}
std::string joke()
{
    std::vector<std::string> jokes;

    jokes.push_back("Why do programmers prefer dark mode? Because light attracts bugs.");
    jokes.push_back("A SQL query walks into a bar, walks up to two tables and asks: 'Can I join you?'");
    jokes.push_back("There are only 10 kinds of people in the world: those who understand binary and those who don't.");
    jokes.push_back("Debugging is like being the detective in a crime movie where you are also the murderer.");
    jokes.push_back("Why did the programmer quit his job? Because he didn't get arrays.");
    jokes.push_back("A programmer's wife tells him: 'Go to the store and buy a loaf of bread. If they have eggs, buy a dozen.' He comes home with 12 loaves of bread.");
    jokes.push_back("Why do Java developers wear glasses? Because they don't C#.");
    jokes.push_back("Programmer: 'It works on my machine.'");
    jokes.push_back("Hardware: the part of the computer you can kick.");
    jokes.push_back("Why was the computer cold? It forgot to close Windows.");
    jokes.push_back("Why don't programmers like nature? Too many bugs.");
    jokes.push_back("How many programmers does it take to change a light bulb? None. It's a hardware problem.");
    jokes.push_back("I would tell you a UDP joke, but you might not get it.");
    jokes.push_back("I would tell you a TCP joke, but I'd have to keep repeating it until you acknowledged it.");
    jokes.push_back("Knock knock. Race condition. Who's there?");
    jokes.push_back("There are two hard things in Computer Science: cache invalidation, naming things, and off-by-one errors.");
    jokes.push_back("Why was the programmer always calm? Because he kept his exceptions handled.");
    jokes.push_back("The best thing about a Boolean is that even if you're wrong, you're only off by a bit.");
    jokes.push_back("My code doesn't have bugs. It develops random features.");
    jokes.push_back("Real programmers count from 0.");
    int index = rand() % jokes.size();
    return jokes[index];
}

std::string quote()
{
    std::vector<std::string> quotes;

    quotes.push_back("The only way to do great work is to love what you do. - Steve Jobs");
    quotes.push_back("Stay hungry, stay foolish. - Steve Jobs");
    quotes.push_back("Talk is cheap. Show me the code. - Linus Torvalds");
    quotes.push_back("Programs must be written for people to read. - Harold Abelson");
    quotes.push_back("Simplicity is the soul of efficiency. - Austin Freeman");
    quotes.push_back("Code never lies, comments sometimes do. - Ron Jeffries");
    quotes.push_back("Knowledge is power. - Francis Bacon");
    quotes.push_back("Success is the sum of small efforts repeated day in and day out. - Robert Collier");
    quotes.push_back("The expert in anything was once a beginner. - Helen Hayes");
    quotes.push_back("Do one thing every day that scares you. - Eleanor Roosevelt");
    quotes.push_back("Believe you can and you're halfway there. - Theodore Roosevelt");
    quotes.push_back("Failure is simply the opportunity to begin again, this time more intelligently. - Henry Ford");
    quotes.push_back("The best way to predict the future is to invent it. - Alan Kay");
    quotes.push_back("First, solve the problem. Then, write the code. - John Johnson");
    quotes.push_back("Experience is the name everyone gives to their mistakes. - Oscar Wilde");
    quotes.push_back("Learning never exhausts the mind. - Leonardo da Vinci");
    quotes.push_back("The journey of a thousand miles begins with one step. - Lao Tzu");
    quotes.push_back("Quality is not an act, it is a habit. - Aristotle");
    quotes.push_back("Dream big and dare to fail. - Norman Vaughan");
    quotes.push_back("Great things are done by a series of small things brought together. - Vincent van Gogh");
    int index = rand() % quotes.size();
    return quotes[index];
}

std::string fact()
{
    std::vector<std::string> facts;

    facts.push_back("Honey never spoils. Archaeologists have found edible honey in ancient Egyptian tombs.");
    facts.push_back("Octopuses have three hearts.");
    facts.push_back("Bananas are berries, but strawberries are not.");
    facts.push_back("A day on Venus is longer than a year on Venus.");
    facts.push_back("Sharks existed before trees.");
    facts.push_back("The Eiffel Tower grows taller in summer because metal expands in the heat.");
    facts.push_back("The human brain contains about 86 billion neurons.");
    facts.push_back("An ostrich's eye is larger than its brain.");
    facts.push_back("There are more stars in the universe than grains of sand on Earth.");
    facts.push_back("Water can boil and freeze at the same time under the right pressure.");
    facts.push_back("A group of flamingos is called a flamboyance.");
    facts.push_back("The shortest war in history lasted about 38 minutes.");
    facts.push_back("The fingerprints of a koala are remarkably similar to those of humans.");
    facts.push_back("The Moon is slowly moving away from Earth at about 3.8 centimeters per year.");
    facts.push_back("The first computer bug was an actual moth found inside a computer.");
    facts.push_back("Some turtles can breathe through their backsides.");
    facts.push_back("A cloud can weigh hundreds of tons.");
    facts.push_back("The hottest planet in our solar system is Venus.");
    facts.push_back("The unicorn is the national animal of Scotland.");
    facts.push_back("There are more possible games of chess than atoms in the observable universe.");
    int index = rand() % facts.size();
    return facts[index];
}

bool calculate(const Message &msg, std::string &result)
{
    if (msg.getParams().size() < 4)
        return false;

    int lhs = std::atoi(msg.getParameter(1).c_str());
    int rhs = std::atoi(msg.getParameter(3).c_str());
    std::string op = msg.getParameter(2);

    std::stringstream ss;

    if (op == "+")
        ss << lhs + rhs;
    else if (op == "-")
        ss << lhs - rhs;
    else if (op == "*")
        ss << lhs * rhs;
    else if (op == "/")
    {
        if (rhs == 0)
        {
            result = "Error: division by zero.";
            return true;
        }
        ss << lhs / rhs;
    }
    else
    {
        result = "Unknown operator.";
        return true;
    }

    result = ss.str();
    return true;
}

void Server::bot(const Message &msg, const Client &client)
{
    if (msg.getParameter(0).empty())
    {
        sendMessageToClient(client.getFd(), botMessage(client, "Need mor parameters "));
        return ;
    }
    if (msg.getParameter(0) ==  "help")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "help here "));
        return ;
    }
    else if (msg.getParameter(0) == "time")
    {
        sendCurrentTime(*this, client);
        return ;
    }
    else if (msg.getParameter(0) == "coin")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "This is your shot : " + coin()));
        return ;
    }
    else if (msg.getParameter(0) == "roll")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "Dice  : " + convertintToString(roll())));
        return ;
    }
    else if (msg.getParameter(0) == "joke")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "Joke : " + joke()));
        return ;
    }
    else if (msg.getParameter(0) == "quote")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "Quote : " + quote()));
        return ;
    }
    else if (msg.getParameter(0) == "fact")
    {
        sendMessageToClient(client.getFd(), botMessage(client, "Fact : " + fact()));
        return ;
    }
    else if (msg.getParameter(0) == "calc")
    {
        std::string answer;

        if (!calculate(msg, answer))
        {
            sendMessageToClient(client.getFd(), botMessage(client, "Usage: calc <number> <+|-|*|/> <number>"));
            return;
        }
        sendMessageToClient(client.getFd(), botMessage(client, answer));
    }
}