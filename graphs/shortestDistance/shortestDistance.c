
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// Use BFS
// Use the visited array as a distance array, update the distance every time
int shortestDistance(Graph g, int src, int dest) {
	
	assert (g != NULL);

	int numNodes = GraphNumVertices(g);
	int dist[numNodes];
	memset(dist, -1, numNodes*sizeof(int));

	dist[src] = 0; // dist fro source to source is zero

	Queue q = QueueNew();
	QueueEnqueue(q, src);

	while (!QueueIsEmpty(q)) {

		int v = QueueDequeue(q);

		for(int w = 0; w < numNodes; w++) {

			if(GraphIsAdjacent(g, v, w) && dist[w] == -1){
				QueueEnqueue(q, w);
				dist[w] = dist[v] + 1;

			}
		}
	}
	
	QueueFree(q);
	return dist[dest];
}

