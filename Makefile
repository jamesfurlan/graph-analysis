CC=clang
CFLAGS=-Wall -Werror -std=c11 -O -lm -o -g
OBJS=Dijkstra.c graph.c PQ.c test.c
BINS=testDij

all :
	$(CC) $(CFLAGS) $(OBJS) -o testDij

clean :
	rm -f $(BINS)

test :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testDij