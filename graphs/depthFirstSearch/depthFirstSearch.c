
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

void recursive(Graph g, int *visited, int v) {

	printf("%d ", v); // prints source
	
	for (int node = 0; w < GraphNumVertices(g); w++) {
		if (GraphIsAdjacent(g, v, w) && visited[w] == -1){ // if node is adjacent and not visited 
			visited[w] = v;
			recursive(g, visited, w);
		}
	}
}

void depthFirstSearch(Graph g, int src) {
	  
	int numNodes = GraphNumVertices(g);
	int visited[numNodes];
	memset(visited, -1, sizeof(int)*numNodes);

	visited[src] = src;	// source
	recursive(g, visited, src);
}

