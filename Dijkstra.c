#include "Dijkstra.h"
#include "PQ.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

ShortestPaths dijkstra(Graph g, Vertex src) {
    assert (g != NULL);
    ShortestPaths dij = {0};
    dij.numNodes = GraphNumVertices(g);
    dij.src = src;

    dij.dist = malloc(sizeof(int) * dij.numNodes);
    dij.pred = malloc(sizeof(PredNode) * dij.numNodes);
    for (int i = 0; i < dij.numNodes; i++) {
        dij.dist[i] = 0;
        dij.pred[i] = NULL;
    }
    dij.dist[src] = 0;

    // array acting as set
    int set[dij.numNodes + 1];
    for (int i = 0; i < dij.numNodes + 1; i++) set[i] = 0;
    PQ toDo = PQNew();
    ItemPQ first = {0};
    first.key = src;
    first.value = dij.dist[src];
    PQAdd(toDo, first);

    ItemPQ v;

    while (set[dij.numNodes] < dij.numNodes && !PQIsEmpty(toDo)) {
        v = PQDequeue(toDo);
        set[v.key] = 1;
        set[dij.numNodes]++;
        for (AdjList outPaths = GraphOutIncident(g, v.key); outPaths != NULL; outPaths = outPaths->next) {
            if (set[outPaths->v]) continue;
            PredNode* newPred = malloc(sizeof(*newPred));
            newPred->v = v.key;
            newPred->next = NULL;
            if (dij.dist[outPaths->v] == 0 || dij.dist[v.key] + outPaths->weight < dij.dist[outPaths->v]) {
                dij.dist[outPaths->v] = dij.dist[v.key] + outPaths->weight;
                PredNode* predCrawl = dij.pred[outPaths->v];
                while (predCrawl != NULL) {
                    PredNode* temp = predCrawl;
                    predCrawl = predCrawl->next;
                    free(temp);
                }
                dij.pred[outPaths->v] = newPred;
            } else if (dij.dist[outPaths->v] == 0 || dij.dist[v.key] + outPaths->weight == dij.dist[outPaths->v]) {
                PredNode* predCrawl = dij.pred[outPaths->v];
                while (predCrawl->next != NULL) predCrawl = predCrawl->next;
                predCrawl->next = newPred;
            } else {
                free(newPred);
                continue;
            }
            ItemPQ next;
            next.key = outPaths->v;
            next.value = dij.dist[outPaths->v];
            PQAdd(toDo, next);
        }
    }

    PQFree(toDo);

    return dij;
}


void showShortestPaths(ShortestPaths sps) {
    printf("numNodes: %d\n", sps.numNodes);
    printf("src: %d\n\n", sps.src);
    for (int i = 0; i < sps.numNodes; i++) {
        if (i == sps.src) printf("Vertex %d\n    dist: X\n", i);
        else printf("Vertex %d\n    dist: %d\n", i, sps.dist[i]);

        PredNode *predCrawl = sps.pred[i];
        printf("    Path: ");
        while (predCrawl != NULL) {
            printf("[%d]->", predCrawl->v);
            predCrawl = predCrawl->next;
        }
        printf("NULL\n\n");
    }
}



void freeShortestPaths(ShortestPaths sps) {
    free(sps.dist);
    for (int i = 0; i < sps.numNodes; i++) {
        PredNode *predCrawl = sps.pred[i];
        while (predCrawl != NULL) {
            PredNode *temp = predCrawl;
            predCrawl = predCrawl->next;
            free(temp);
        }
    }
    free(sps.pred);
}
