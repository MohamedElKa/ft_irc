NAME = ircserv
CPPFLAGS = -Wextra -Wall -Werror -std=c++98
SRCS = Bot.cpp channel.cpp command.cpp invite.cpp kick.cpp mode.cpp topic.cpp part.cpp nick.cpp execute.cpp join.cpp main.cpp utils.cpp \
	quit.cpp private_msg.cpp Launch.cpp server.cpp notice.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = Client.hpp  channel.hpp command.hpp ft_irc.hpp  server.hpp

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(HEADERS)
	c++  $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean re all fclean
