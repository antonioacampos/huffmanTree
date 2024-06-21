CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm
DEPS = huffman.h
OBJ = main.o huffman.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

huffman: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o huffman
