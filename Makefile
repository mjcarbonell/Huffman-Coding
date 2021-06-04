CC = clang 
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g

all: encode decode entropy

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o encode encode.o node.o pq.o code.o io.o stack.o huffman.o -lm
decode: decode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o decode decode.o node.o pq.o code.o io.o stack.o huffman.o -lm

entropy: entropy.o
	$(CC) -o entropy entropy.o -lm

node.o: node.c
	$(CC) $(CFLAGS) -c node.c
pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c
code.o: code.c
	$(CC) $(CFLAGS) -c code.c
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c
huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c
encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c
decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c


clean:
	rm -f encode encode.o decode.o node.o code.o pq.o stack.o  huffman.o  

scan-build: clean
	scan-build make   
	
format: 
	clang-format -i -style=file encode.c encode.h decode.c node.c node.h code.c code.h stack.c stack.h  huffman.c huffman.h  pq.c pq.h io.c io.h  
