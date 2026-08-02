CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/Client.cpp src/main.cpp src/Server.cpp src/Message.cpp src/Parser.cpp src/Reply.cpp src/Channel.cpp src/bot.cpp \
		src/cmds/JOIN.cpp src/cmds/INVITE.cpp src/cmds/KICK.cpp src/cmds/LIST.cpp src/cmds/MODE.cpp src/cmds/MsgCommands.cpp src/cmds/NICK.cpp src/cmds/PART.cpp src/cmds/PASS.cpp src/cmds/TOPIC.cpp src/cmds/USER.cpp

NAME = ircserv

all : $(NAME)

$(NAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

clean :
	rm -f $(NAME)
fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
