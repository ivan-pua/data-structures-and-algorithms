// Lance-Williams Algorithm for Hierarchical Agglomerative Clustering
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BSTree.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "LanceWilliamsHAC.h"
#define INF 9999.99

static Dendrogram createDendogramNode(int v);
static double** createArray(int n);
static void destroyArray(double** arr);
static void showDist(double **dist, int numVertices);
static double min(double **dist, int p, int q, int rowNum); 
/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */

Dendrogram LanceWilliamsHAC(Graph g, int method) {

    printf(" HELLO \n");
    int numVertices = GraphNumVertices(g);
    int numVertices2 = numVertices;

    double **DIST = createArray(numVertices); // empty array

    for (int i = 0; i < numVertices; i++) {

        AdjList outLinks = GraphOutIncident(g, i);
        AdjList inLinks = GraphInIncident(g, i);

        for (int j = 0; j < numVertices; j++) {
            
            double max = 0.0;
            while (outLinks != NULL) {
                
                if (outLinks->weight > max){
                    max = outLinks->weight;
                }
                outLinks = outLinks->next;
            }

            while (inLinks != NULL) {
                
                if (inLinks->weight > max){
                    max = inLinks->weight;
                }
                inLinks = inLinks->next;
            }          

            if (max == 0) {
                DIST[i][j] = DIST[j][i] = INF;
            } else {
                DIST[i][j] = DIST[j][i] = 1.0/max;

            }
            DIST[j][j] = 0.0;
        }
    }

    showDist(DIST, numVertices);

    // Initiliaze an array of dendrogram pointers
    Dendrogram *dendA;
    dendA = calloc(numVertices, sizeof(Dendrogram*));

    dendA= calloc(numVertices, sizeof(Dendrogram*));
	for (int i = 0; i < numVertices; i++) {
		dendA[i] = NULL;
	}
	for (int i = 0; i < numVertices; i++) {
        Dendrogram d = createDendogramNode(i);
        dendA[i] = d;
	}

    Dendrogram *updatedDendA;
    updatedDendA= calloc(numVertices, sizeof(Dendrogram*));
	for (int i = 0; i < numVertices; i++) {
		updatedDendA[i] = NULL;
	}   

    // iterates until last 
    for (int k = 1; k < (numVertices2) - 1; k++) {

        // Find two closest clusters and min distance
        int minDist, i, j = 0;
        for (int row = 0; row < numVertices; row++)  { 

            for (int col = 0; col <= numVertices/2; col++) { 
                // Compare elements from beginning and end of current row 
                if (DIST[row][col] > DIST[row][numVertices-col-1])  { 
                    if (minDist > DIST[row][numVertices-col-1]) {
                        minDist = DIST[row][numVertices-col-1]; 
                        i = row;
                        j = numVertices-col-1;
                    } 
                } else { 
                    if (minDist > DIST[row][col]) {
                        minDist = DIST[row][col];
                        i = row;
                        j = numVertices-col-1;
                    }  
                } 
            } 
        }

        // Decrease array size by 1 - IMPORTANT
        numVertices--;

        /* ----------- UPDATE DIST ARRAY ---------- */

        // Make a new array with a smaller size
        double **updatedDIST = createArray(numVertices);

        int updatedRow = 0;
        for (int row = 0; row < numVertices-1; row++)  { 
            
            // Compare elements from beginning and end of current row 
            if (row == i || row == j) {
                row++;
                continue;
            }

            int updatedCol = 0;
            for (int col = 0; col < numVertices; col++) { 

                if (DIST[row][col] == i || DIST[row][col] == j) {
                    col++;
                    continue;
                }

                updatedDIST[updatedRow][updatedCol] = DIST[row][col];
                updatedCol++;
            }
            updatedRow++;
        }
        

        showDist(updatedDIST, numVertices);


        // Lance Williams Algo
        // i, j represents the vertices that has the smallest distance in between
         // Add new cluster at the last row of the array
        int row = 0;
        for (int updatedCol = 0; updatedCol < numVertices; updatedCol++)  {
           
            if (updatedCol == numVertices -1) {
                updatedDIST[updatedRow][updatedCol] = 0;
                updatedCol --;
                continue; 
            }

            // If the row counter of the original DIST array is equals to i or j 
            //(the 2 rows with the smallest distance), skip
            if(row == i || row == j) {
                continue;
            }

            updatedDIST[updatedRow][updatedCol] = min(DIST, i, j, row);
            row++;
        }

        showDist(updatedDIST, numVertices);

        /* ----------- UPDATE DENDOGRAM ARRAY ---------- */
        // Merge clusters in updated dendogram array
        int originalDendCount = 0;
        int dendCount = 0;
        for (dendCount = 0; dendCount < numVertices-1; dendCount++) {

            if (originalDendCount == i || originalDendCount == j) {
                originalDendCount++;
                dendCount --;
                continue;
            } 
            updatedDendA[dendCount]= dendA[originalDendCount];
        }


        // update last column of dendrogram array
        Dendrogram cluster = updatedDendA[dendCount];
        cluster->vertex = -1; // does not matter what cluster 
        cluster->left = dendA[i];
        cluster->right = dendA[j];

        // Equate original array with the updated array, ignore the last column as the size - 1
        dendCount = 0;
        for (originalDendCount = 0; originalDendCount < numVertices; originalDendCount++) {
            dendA[originalDendCount] = updatedDendA[dendCount];
        } 

        // double **DIST = createArray(numVertices);

        // update the original array for the next iteration
        for (i= 0; i < numVertices; i++) {

            for ( j = 0; j < numVertices; j++) {
                DIST[i][j] = updatedDIST[i][j];
            }
        }

        destroyArray(updatedDIST);

    }
    return updatedDendA[0];
}

/**
 * Frees all memory associated with the given Dendrogram structure.
 */
 void freeDendrogram(Dendrogram d) {

 }

static double** createArray(int n)
{
    double* values = calloc(n, sizeof(double));
    double** rows = calloc(n*n, sizeof(double));
    for (int i=0; i<n; ++i)
    {
        rows[i] = values;
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            rows[i][j] = INF; // INF = 9999.99
        }
    }

    return rows;
}

static void destroyArray(double** arr)
{
    free(*arr);
    free(arr);
}

static void showDist(double **dist, int numVertices) {

    // Check DIST array 
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%f ", dist[i][j]);
        }
        printf("\n");
    }

}

static double min(double **dist, int p, int q, int rowNum) {

    if (dist[rowNum][p] == INF && dist[rowNum][q] == INF) {
        return INF;
    }

    if (dist[rowNum][p] >= dist[rowNum][q]) {
        return dist[rowNum][q];
    } else {
        return dist[rowNum][p];
    }
}

static Dendrogram createDendogramNode(int v) {

    Dendrogram n = malloc(sizeof *n);
    n->vertex = v;
    n->left = n->right = NULL;
    return n;
}

