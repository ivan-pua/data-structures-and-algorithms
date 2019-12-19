
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// BFS
int furthestReachable(Graph g, int src) {
	
	int numNodes = GraphNumVertices(g);
	int dist[numNodes];
	memset(dist, -1, numNodes*sizeof(int));

	dist[src] = 0;

	Queue q = QueueNew();
	QueueEnqueue(q, src);

	while (!QueueIsEmpty(q)) {
		int v =QueueDequeue(q);

		for(int w = 0; w < numNodes; w++) {
			if(GraphIsAdjacent(g,v,w) && dist[w] == -1) {
				dist[w] = dist[v] + 1;
				QueueEnqueue(q, w);
			}
		}
	}
	int largestDist = -1;
	int vertexWithLargestDist = 0;
	for(int i = 0; i< numNodes; i++) {
		if(dist[i] >= largestDist) {
			largestDist = dist[i];
			vertexWithLargestDist = i;
		}

	}

	QueueFree(q);
	return vertexWithLargestDist;
}

