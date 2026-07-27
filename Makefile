CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/Client.cpp src/main.cpp src/Server.cpp src/Message.cpp src/Parser.cpp src/cmds/* src/Reply.cpp src/Channel.cpp

SRCC = src/mync.cpp
NAME = ircserv
NAMEC = client

all : $(NAME)

$(NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

fclean :
	rm -f $(NAME)

re : fclean $(NAME)

.PHONY : all NAME fclean re
