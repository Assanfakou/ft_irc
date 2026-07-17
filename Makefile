CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/Client.cpp src/main.cpp src/Server.cpp src/Message.cpp src/Parser.cpp src/cmds/* src/Reply.cpp src/Channel.cpp

SRCC = src/mync.cpp
NAME = server
NAMEC = client

all : $(NAME)

$(NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)
	$(CXX) $(CXXFLAGS) $(SRCC) -o $(NAMEC)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

fclean :
	rm -f $(NAME) $(NAMEC)

re : fclean $(NAME) $(NAMEC)

.PHONY : all NAME fclean re
