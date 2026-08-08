
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = bot/bot.cpp bot/botClient.cpp   Client.cpp Server.cpp Channel.cpp Message.cpp Parser.cpp Reply.cpp \
	cmds/JOIN.cpp cmds/INVITE.cpp cmds/KICK.cpp cmds/LIST.cpp cmds/MODE.cpp cmds/MsgCommands.cpp \
	cmds/NICK.cpp cmds/PART.cpp cmds/PASS.cpp cmds/TOPIC.cpp cmds/USER.cpp

OBJ = $(patsubst %.cpp,testing/%.o, $(SRC))

NAME = Bot

all : $(NAME)

$(NAME): $(OBJ) 
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	
testing/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)
	@rm -rf testing

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all NAME clean fclean re
.SECONDARY: $(OBJ) 
