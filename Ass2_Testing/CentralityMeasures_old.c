// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

NodeValues closenessCentrality(Graph g) {
	NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(sizeof(double) * nvs.numNodes);
    for (int i = 0; i < nvs.numNodes; i++) {
        if (nvs.numNodes == 1) {
            nvs.values[i] = 0;
            continue;
        }
        int reachable = 0;
        double shortSum = 0;
        ShortestPaths dij = dijkstra(g, i);
        for (int j = 0; j < nvs.numNodes; j++) {
            shortSum += dij.dist[j];
            if (dij.dist[j] != 0) reachable++;
        }
        double closeness = 0;
        if (shortSum != 0)
            // reachable is already subtracted by one since the node itself wasn't counted
            closeness = ((double)(reachable)/(nvs.numNodes - 1)) * ((double)(reachable)/shortSum);
        nvs.values[i] = closeness;
        freeShortestPaths(dij);
    }

	return nvs;
}

NodeValues betweennessCentrality(Graph g) {
	NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(sizeof(double) * nvs.numNodes);
    for (int i = 0; i < nvs.numNodes; i++) { // central node
        // hold sum
        double sum = 0;
        for (int j = 0; j < nvs.numNodes; j++) { // dijkstra'd node
            if (j == i) continue;
            // dijkstra
            ShortestPaths dij = dijkstra(g, j);
            for (int k = 0; k < nvs.numNodes; k++) {
                if (k == i || k == j) continue;
                // hold total shortest paths
                int shortTotal = 0;
                // hold total shortest paths w/ member
                int includeTotal = 0;
                // increment shortest paths
                PredNode* predCrawl = dij.pred[k];
                while (predCrawl != NULL) {
                    shortTotal++;
                    PredNode* predCreep = predCrawl;
                    while (predCreep != NULL) {
                        if (predCreep->v == i) includeTotal++; 
                        predCreep = dij.pred[predCreep->v];
                    }
                    predCrawl = predCrawl->next;
                }
                if (shortTotal != 0)
                    sum += (double)includeTotal/shortTotal;
                // increment shortest paths w/ member
            }
            // do maths and add to sum
                // remember to check for 0'd denominators
            
            // free dijkstra
            freeShortestPaths(dij);
        }
        // store sum
        nvs.values[i] = sum;
    }

	return nvs;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	NodeValues nvs = {0};
	return nvs;
}

void showNodeValues(NodeValues nvs) {
    // printf("numNodes: %d\n", nvs.numNodes);
    for (int i = 0; i < nvs.numNodes; i++)
        printf("%d: %lf\n", i, nvs.values[i]);
}

void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}

