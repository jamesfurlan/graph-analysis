CC=clang
CFLAGS=-Wall -Werror -std=c11 -g

all :
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c testDij.c -o testDij
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c CentralityMeasures.c testClose.c -o testClose
	$(CC) $(CFLAGS) graph.c LanceWilliamsHAC.c testHAC.c -o testHAC

dijkstra:
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c testDij.c -o testDij

centrality:
	$(CC) $(CFLAGS) Dijkstra.c graph.c PQ.c CentralityMeasures.c testClose.c -o testClose

hac:
	$(CC) $(CFLAGS) graph.c LanceWilliamsHAC.c testHAC.c -o testHAC

clean :
	rm -f testDij testClose testHAC

testDijkstra :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testDij

testCentrality :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testClose

testHAC :
	make && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./testHAC
