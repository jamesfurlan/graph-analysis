#include <stdio.h>
#include "LanceWilliamsHAC.h"

int main(void) {
    Graph g = GraphNew(3);
    GraphInsertEdge(g, 0, 1, 2);
    GraphInsertEdge(g, 0, 2, 1);
    GraphInsertEdge(g, 1, 0, 2);
    GraphInsertEdge(g, 2, 1, 1);
    GraphShow(g);
    printf("===========\n");
    Dendrogram a = LanceWilliamsHAC(g, 1);
    freeDendrogram(a);
    GraphFree(g);
}