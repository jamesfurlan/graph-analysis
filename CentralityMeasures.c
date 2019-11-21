// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

static void recursiveCounter(PredNode** pred, PredNode* start, Vertex target, int* totalCount, int* VIPcount, int VIPflag);

NodeValues closenessCentrality(Graph g) {
	NodeValues nvs = {0};
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(sizeof(double) * nvs.numNodes);
    for (int i = 0; i < nvs.numNodes; i++) {
        // To avoid later attempts to divide by 0
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

                int totalCount = 0;
                int VIPcount = 0;

                recursiveCounter(dij.pred, dij.pred[k], i, &totalCount, &VIPcount, 0);

                if (totalCount != 0)
                    sum += (double)VIPcount/totalCount;
                    
            }
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
    // call betweennessCentrality and then do the division on each value
    nvs = betweennessCentrality(g);
    if (nvs.numNodes <= 2) return nvs; 
    for (int i = 0; i < nvs.numNodes; i++) {
        nvs.values[i] = nvs.values[i]/((nvs.numNodes - 1)*(nvs.numNodes - 2));
    }
	return nvs;
}

void showNodeValues(NodeValues nvs) {
    for (int i = 0; i < nvs.numNodes; i++)
        printf("%d: %lf\n", i, nvs.values[i]);
}

void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}

// would be better to change this later to be more algorithmically efficient
// currently goes through every pair of nodes n times - could go through once storing all n data as it goes in an array
static void recursiveCounter(PredNode** pred, PredNode* start, Vertex target, int* totalCount, int* VIPcount, int VIPflag) {
    // At the end of a route it increments the required counters
    if (start == NULL) {
        (*totalCount)++;
        if (VIPflag) (*VIPcount)++;
        return;
    }
    // Goes through all side pred parts that have equal shortest distances
    if (start->next != NULL) recursiveCounter(pred, start->next, target, totalCount, VIPcount, VIPflag);
    // Sets the VIP flag to true so that when it gets to the end all branching 
    // routes from here will be acknowledged as having passed the required node
    if (start->v == target) VIPflag = 1;
    // Moves to the next in the pred route
    recursiveCounter(pred, pred[start->v], target, totalCount, VIPcount, VIPflag);
}