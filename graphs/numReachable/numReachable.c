
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

void recursive(Graph g, int *visited, int v) {

	for(int w = 0; w < GraphNumVertices(g); w++) {

		if(GraphIsAdjacent(g, v, w) && visited[w] == -1) {
			visited[w] = 1;
			recursive(g, visited, w);
		} 
	}
}
int numReachable(Graph g, int src) {
	
	assert (g  != NULL);

	int numNodes = GraphNumVertices(g);
	int visited[numNodes];
	memset(visited, -1, numNodes*sizeof(int));
	visited[src] = 1;

	recursive(g, visited, src);

	int reachable = 0;
	// if visited[i] = 1, means its reachable
	for (int i = 0; i < numNodes; i++) {
		if(visited[i] == 1){
			reachable++;
		}
	}
	return reachable;
}

