NAME = minishell
CC = gcc
LDFLAGS = "-L/opt/homebrew/opt/readline/lib"
CPPFLAGS="-I/opt/homebrew/opt/readline/include"
SRCS = envp_convert.c\
		envp_list.c\
		list.c\
		main.c\
		parser.c\
		signal.c\
		util.c

OBJS = $(SRCS:.c=.o)

$(NAME) : $(OBJS)
	$(CC) -lreadline $(LDFLAGS) $(CPPFLAGS) -lft -L./libft -o $(NAME) $(OBJS) 

all: $(NAME)

clean :
		rm -rf $(OBJS)

fclean : clean
		rm -rf $(OBJS)

re : fclean $(NAME)

PHONY : all clean fclean re
