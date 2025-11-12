CC = gcc
CFLAGS = -Wall
SRCS = main_shell.c week2_shell.c week3_shell.c week4_shell.c week5_shell.c week6_shell.c exec_demo.c fork_demo.c parser_demo.c
OBJ = $(SRCS:.c=.o)
all: shell
shell: $(OBJ)
	$(CC) $(CFLAGS) -o shell $(OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o shell
