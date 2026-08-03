CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/Client.cpp src/main.cpp src/Server.cpp src/Message.cpp src/Parser.cpp src/Reply.cpp src/Channel.cpp src/bot.cpp \
		src/cmds/JOIN.cpp src/cmds/INVITE.cpp src/cmds/KICK.cpp src/cmds/LIST.cpp src/cmds/MODE.cpp src/cmds/MsgCommands.cpp src/cmds/NICK.cpp src/cmds/PART.cpp src/cmds/PASS.cpp src/cmds/TOPIC.cpp src/cmds/USER.cpp

OBJ = $(patsubst %.cpp,builds/%.o, $(SRC))

NAME = ircserv

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

builds/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean :
	rm -f $(OBJ)
	@rm -rf builds

fclean : clean
	rm -f $(NAME)

re : fclean $(NAME)

.PHONY : all NAME clean fclean re
.SECONDARY: $(OBJ)

