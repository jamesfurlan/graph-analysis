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
        // printf("i = %d\n", i);
        double sum = 0;
        for (int j = 0; j < nvs.numNodes; j++) { // dijkstra'd node
            if (j == i) continue;
            // printf("j = %d\n", j);
            // dijkstra
            ShortestPaths dij = dijkstra(g, j);
            // hold total shortest paths
            // hold total shortest paths w/ member
            for (int k = 0; k < nvs.numNodes; k++) {
                if (k == i || k == j) continue;

                int totalCount = 0;
                int VIPcount = 0;

                recursiveCounter(dij.pred, dij.pred[k], i, &totalCount, &VIPcount, 0);

                if (totalCount != 0)
                    sum += (double)VIPcount/totalCount;
                if (VIPcount) {
                    printf("passi = %d numOfPaths = %d\n", VIPcount, totalCount);
                    printf("i = %d j = %d k = %d sum = %lf\n", i, j, k, sum);
                }
                // shortTotal += totalCount;
                // includeTotal += VIPcount;

                // increment shortest paths
                // need a way of going through every path possible
                    // recursion?
                    // for each level go through all options on sides
                    // as well as all layers


                // PredNode* predCrawl = dij.pred[k];
                // while (predCrawl != NULL) {
                //     shortTotal++;
                //     PredNode* predCreep = predCrawl;
                //     while (predCreep != NULL) {
                //         if (predCreep->v == i) includeTotal++; 
                //         predCreep = dij.pred[predCreep->v];
                //     }
                //     predCrawl = predCrawl->next;
                // }



                // increment shortest paths w/ member
            }
            // do maths and add to sum
                // remember to check for 0'd denominators
            // if (shortTotal != 0)
                // sum += (double)includeTotal/shortTotal;
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
    // call betweennessCentrality and then do the division on each value?
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

// am i actually a genius? why am i dumb
static void recursiveCounter(PredNode** pred, PredNode* start, Vertex target, int* totalCount, int* VIPcount, int VIPflag) {
    if (start == NULL) {
        (*totalCount)++;
        if (VIPflag) (*VIPcount)++;
        return;
    }
    
    // !!! OH OH OH ITS HERE
    // ! IT'S CALLING MORE NEXTS WHICH CAUSES REPEATS - ONLY CALL IT ONCE
    // RECURSION SUCKS DICK
    // PredNode* predCrawl = start->next;
    // while (predCrawl != NULL) {
    //     recursiveCounter(pred, predCrawl, target, totalCount, VIPcount, VIPflag);
    //     predCrawl = predCrawl->next;
    // }
    if (start->next != NULL) recursiveCounter(pred, start->next, target, totalCount, VIPcount, VIPflag);

    if (start->v == target) VIPflag = 1;

    recursiveCounter(pred, pred[start->v], target, totalCount, VIPcount, VIPflag);
}
