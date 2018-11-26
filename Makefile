RM	=	rm -f

CFLAGS	=	-g3 -W -Wall -Wextra -I include -fdiagnostics-color=auto

#LIBS   =       -lcsfml-window -lcsfml-graphics
LIBS	=	-lm -lc_graph_prog

SRC	=	src/main.c

OBJ	=	$(SRC:.c=.o)

EXEC	=	vill_gen

all:		$(EXEC)

$(EXEC):	$(OBJ)
		$(CC) $(OBJ) -o $(EXEC) $(LIBS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(EXEC)

re: fclean all
