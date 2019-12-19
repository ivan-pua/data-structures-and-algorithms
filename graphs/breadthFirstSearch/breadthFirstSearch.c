
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

void breadthFirstSearch(Graph g, int src) {
	
	assert (g != NULL);

	int numNodes = GraphNumVertices(g);
	int visited[numNodes];
	memset(visited, -1, numNodes*sizeof(int));
	visited[src] =src;

	Queue q = QueueNew();
	QueueEnqueue(q, src);

	while (!QueueIsEmpty(q)) {

		int v = QueueDequeue(q);
		printf("%d ", v);

		for (int w = 0; w < numNodes; w++) {

			if(GraphIsAdjacent(g, v, w) && visited[w] == -1) {
				visited[w] = v;
				QueueEnqueue(q, w);
			}

		}
	}

	QueueFree(q);
	// free(visited);
	
}

