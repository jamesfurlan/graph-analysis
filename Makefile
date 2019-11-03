CC=clang
CFLAGS=-Wall -Werror -std=c11 -O -lm -o -g
OBJS=Dijkstra.c graph.c PQ.c testDij.c
BINS=testDij

all :
	$(CC) $(CFLAGS) $(OBJS) -o testDij
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c CentralityMeasures.c testClose.c -o testClose

dijkstra:
	$(CC) $(CFLAGS) $(OBJS) -o testDij

centrality:
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c CentralityMeasures.c testClose.c -o testClose

clean :
	rm -f testDij testClose 

testDijkstra :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testDij

testCentrality :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testClose
