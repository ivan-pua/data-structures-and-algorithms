
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// BFS
int numWithin(Graph g, int src, int dist) {
	
	int numNodes = GraphNumVertices(g);
	int distArray[numNodes];
	memset(distArray, -1, numNodes*sizeof(int));
	distArray[src] = 0;

	Queue q = QueueNew();
	QueueEnqueue(q, src);

	while (!QueueIsEmpty(q)) {

		int v = QueueDequeue(q);

		for(int w = 0; w < numNodes; w++) {

			if(GraphIsAdjacent(g, v, w) && distArray[w] == -1) {
				distArray[w] = distArray[v] + 1; 
				QueueEnqueue(q, w);
			}

		}

	}

	int num = 0;
	for (int i = 0; i < numNodes; i++) {
		if(distArray[i] >= 0 && distArray[i] <= dist) {
			num++;
		}
	}
	
	QueueFree(q);

	return num;
}

