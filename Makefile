RM	=	rm -f

CFLAGS	=	-g3 -W -Wall -Wextra -I include

#CSFML   =       -lcsfml-window -lcsfml-graphics
CSFML	=	-lc_graph_prog

SRC	=	src/main.c

OBJ	=	$(SRC:.c=.o)

EXEC	=	vill_gen

all:		$(EXEC)

$(EXEC):	$(OBJ)
		$(CC) $(OBJ) -o $(EXEC) $(CSFML)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(EXEC)

re: fclean all
