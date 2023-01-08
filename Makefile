SOURCES_FILES	=	main.c \
					utils.c


SOURCES_DIR		=	srcs

HEADER			=	include.h

SOURCES			=	$(addprefix $(SOURCES_DIR)/, $(SOURCES_FILES))

OBJECTS			= 	$(SOURCES:.c=.o)

NAME			=	ft_malcolm

CC				=	gcc
RM				=	rm -f

.c.o:
				$(CC) -c $< -o $(<:.c=.o)

all:			$(NAME)


$(NAME):		$(OBJECTS) $(HEADER)
				$(CC) $(OBJECTS) -o $(NAME)

clean:
				$(RM) $(OBJECTS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re bonus