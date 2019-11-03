#include <stdio.h>
#include "Dijkstra.h"

int main(void) {
    Graph g = GraphNew(3);
    GraphInsertEdge(g, 0, 1, 2);
    GraphInsertEdge(g, 0, 2, 1);
    GraphInsertEdge(g, 1, 0, 2);
    GraphInsertEdge(g, 2, 1, 1);
    GraphShow(g);
    printf("===========\n");
    ShortestPaths test = dijkstra(g, 1);
    showShortestPaths(test);
    GraphFree(g);
    freeShortestPaths(test);
}