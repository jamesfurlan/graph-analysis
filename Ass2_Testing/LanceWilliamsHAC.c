// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

#define MAX_INT 2147483647 // nicer than an extra (x != -1 || x < ...)

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    assert(method == SINGLE_LINKAGE || method == COMPLETE_LINKAGE);
    // Calculate distances between each pair of vertices
    int vertCount = GraphNumVertices(g);
    double distance[vertCount][vertCount];
    Dendrogram dendA[vertCount];
    for (int i = 0; i < vertCount; i++) {
        for (int j = i; j < vertCount; j++) {
            if (!GraphIsAdjacent(g, i, j) && !GraphIsAdjacent(g, j, i)) {
                distance[i][j] = distance[j][i] = MAX_INT;
                continue;
            }
            // find distance and calculate
            int outWeight = 0;
            int inWeight = 0;
            for (AdjList iOut = GraphOutIncident(g, i); iOut != NULL; iOut = iOut->next)
                if (iOut->v == j) outWeight = iOut->weight;
            for (AdjList iIn = GraphInIncident(g, i); iIn != NULL; iIn = iIn->next)
                if (iIn->v == j) outWeight = iIn->weight;

            distance[i][j] = distance[j][i] = (double)1/(outWeight - inWeight > 0 ? outWeight : inWeight);
        }
        Dendrogram dendNew = malloc(sizeof(*dendNew));
        dendA[i] = dendNew;
        dendA[i]->vertex = i;
        dendA[i]->left = NULL;
        dendA[i]->right = NULL;
    }

    for (int i = 1; i < vertCount; i++) {
        double min = MAX_INT;
        int vert1 = 0;
        int vert2 = 0;
        for (int j = 0; j < vertCount + 1 - i; j++) {
            for (int k = j; k < vertCount + 1 - i; k++) {
                if (distance[j][k] <= min) {
                    if (j == k) continue;
                    min = distance[j][k];
                    vert1 = j;
                    vert2 = k;
                }
            }
        }
        Dendrogram ci = dendA[vert1];
        Dendrogram cj = dendA[vert2];
        for (int j = vert1; j < vertCount - 1; j++)
            dendA[j] = dendA[j + 1];
        int second = vert2;
        if (vert2 > vert1) second--;
        for (int j = second; j < vertCount - 1; j++)
            dendA[j] = dendA[j + 1];

        Dendrogram dendNew = malloc(sizeof(*dendNew));
        dendA[vertCount - 1 - i] = dendNew;
        dendA[vertCount - 1 - i]->vertex = -1;
        dendA[vertCount - 1 - i]->left = ci;
        dendA[vertCount - 1 - i]->right = cj;

        // calculate distance to new cluster
        double newDist[vertCount - i];
        int index = 0;
        for (int j = 0; j < vertCount - i + 1; j++) {
            if (j == vert1 || j == vert2) continue;
            if (method == SINGLE_LINKAGE)
                newDist[index] = (distance[vert1][j] < distance[vert2][j] ? distance[vert1][j] : distance[vert2][j]);
            else
                newDist[index] = (distance[vert1][j] > distance[vert2][j] ? distance[vert1][j] : distance[vert2][j]); 
            index++;
        }
        newDist[vertCount - i - 1] = MAX_INT;

        // remove old 2 clusters distances and insert new one
        for (int j = vert1; j < vertCount; j++)
            for (int k = 0; k < vertCount; k++) {
                distance[j][k] = distance[j + 1][k];
                distance[k][j] = distance[k][j + 1];
            }
        for (int j = second; j < vertCount; j++)
            for (int k = 0; k < vertCount; k++) {
                distance[j][k] = distance[j + 1][k];
                distance[k][j] = distance[k][j + 1];
            }

        for (int j = 0; j < vertCount - i; j++)
            distance[j][vertCount - i - 1] = distance[vertCount - i - 1][j] = newDist[j];

    }

    return dendA[0];
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    if (d == NULL) return;
    freeDendrogram(d->left);
    freeDendrogram(d->right);
    free(d);
}