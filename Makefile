OBJ=main
FILES=main.c dynamic_que.c uart.c
all:
	$(CC) -o $(OBJ) $(FILES)