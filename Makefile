SRC =	main.cpp			\
		Server.cpp			\
		Client.cpp			\
		Channel.cpp			\
		tools.cpp			\
		commands.cpp		\
		cmds/USER.cpp		\
		cmds/NICK.cpp		\
		cmds/JOIN.cpp		\
		cmds/CAP.cpp		\
		cmds/PRIVMSG.cpp	\
		cmds/ISON.cpp		\
		cmds/WHO.cpp		\
		cmds/QUIT.cpp		\
		cmds/PASS.cpp		\
		cmds/PART.cpp		\
		cmds/NOTICE.cpp		\
		cmds/TOPIC.cpp		\
		cmds/INVITE.cpp		\
		cmds/MODE.cpp		\
		cmds/KICK.cpp		\
		cmds/BAN.cpp		\
		cmds/OP.cpp			\
		cmds/VOICE.cpp		\
		cmds/BOT.cpp		\
		cmds/PING.cpp		\
		cmds/EXITS.cpp

OBJ = $(SRC:.cpp=.o)
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@c++ $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[05;92m-> $(NAME) successfully compiled <-\033[0;39m"

clean:
	@rm -f $(OBJ)
	@echo "\033[0;92m.o files cleaned.\033[0;39m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[0;92m$(NAME) files cleaned.\033[0;39m"

re: fclean $(NAME)
	@echo "\033[0;92mCleaned and rebuilded.\033[0;39m"

.PHONY: all clean fclean re
