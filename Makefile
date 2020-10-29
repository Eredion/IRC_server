NAME = ircserv

SRC =	src/main.cpp src/Server.cpp src/libft.cpp src/utils.cpp src/Client.cpp \
		src/Message.cpp src/commands.cpp src/Channel.cpp src/ssl.cpp

INC =	inc/ft_irc.hpp inc/Server.hpp inc/reply.hpp inc/Client.hpp \
		inc/Message.hpp inc/Channel.hpp

SSL = -L/Users/${USER}/.brew/Cellar/openssl@1.1/1.1.1g/lib -lssl -lcrypto
SSLI = -I/Users/${USER}/.brew/Cellar/openssl@1.1/1.1.1g/include

COMPILER = clang++
$(NAME): $(SRC) $(INC)
	@sh ip.sh
	@sleep 1
	$(COMPILER) -Wall -Wextra -Werror $(SRC) $(SSL) $(SSLI) -o $(NAME)
	@cp inc/ft_irc.hpp.bck inc/ft_irc.hpp

all: $(NAME)

clean:
	rm -f inc/ft_irc.hpp.bck

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

bonus: re all

push: $(SRC) $(INC) fclean
	git add $(SRC) $(INC) Makefile .gitignore
	git commit -m "ft_irc darodrig asegovia"
	git push

x: re
	./ircserv 6667 0

y: re
	./ircserv 6669 0

z:
	./ircserv localhost:6667:0 6668 0

tn:
	telnet irc.freenode.org 6667