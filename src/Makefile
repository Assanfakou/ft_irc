
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = bot/bot.cpp bot/botClient.cpp cmds/* Client.cpp Server.cpp Channel.cpp Message.cpp Parser.cpp Reply.cpp 

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
	@rm -rf builds

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all NAME clean fclean re
.SECONDARY: $(OBJ) 
