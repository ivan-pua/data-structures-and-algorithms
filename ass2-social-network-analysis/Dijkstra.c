// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

#define INT_MAX 2147483646 // Largest possible number of int

static ItemPQ createItem(Vertex n, int weight);
static PredNode *createPredNode(Vertex v);

ShortestPaths dijkstra(Graph g, Vertex src) {
	
	ShortestPaths sps = {0};

	sps.src = src;
	sps.numNodes = GraphNumVertices(g); 	
	sps.dist = calloc(sps.numNodes, sizeof(int));

	// Initialise all values in dist[] to the largest number
	for (int i = 0; i < sps.numNodes; i++) {	
		sps.dist[i] = INT_MAX;
	}
	// The distance of the source element would be 0
	sps.dist[src] = 0;

	// Initialise all elements in predecessor array to be NULL
	sps.pred= calloc(sps.numNodes, sizeof(PredNode));
	for (int i = 0; i < sps.numNodes; i++) {
		sps.pred[i] = NULL;
	}


	// Create an empty priority queue pq.  
	// Every item of pq is a pair (value, key). 
	PQ pq = PQNew();

	// Insert source vertex into pq and make its distance as 0,
	// Because the distance between source is 0.
	ItemPQ source= createItem(src, sps.dist[src]);
	PQAdd(pq, source);	

	// While either pq doesn't become empty
	while (!PQIsEmpty(pq)) {
		
		// Extract minimum distance vertex from pq. 
		// Let the extracted vertex be u.
		ItemPQ u = PQDequeue(pq);

		// Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values 
		AdjList adjacents = GraphOutIncident(g, u.key);
		while (adjacents != NULL)  {

			// adjacentNodeID is the nodes that are adjacent to node u 
			int adjacentNodeID = adjacents->v;

			// Case where we found another shortest path with the same weight
			if (sps.dist[adjacentNodeID] == adjacents->weight + sps.dist[u.key]) {

				// Create a new node			
				PredNode *n = createPredNode(u.key);
				PredNode *curr = sps.pred[adjacentNodeID];
				
				// Traverse to the end of linked list
				while (curr->next!= NULL) {
					curr = curr->next;
				}
				// Append the node on pred array
				curr->next = n;
			}

			/**
			 * ------ Edge Relaxation ------
			 * if there is a shorter path to v through u, update. 
			 * Theory: dist[v] > dist[u] + weight(u, v)
			 */
			if (sps.dist[adjacentNodeID] > adjacents->weight + sps.dist[u.key]) {
				
				// Update distance to v
				sps.dist[adjacentNodeID] = adjacents->weight + sps.dist[u.key]; 
				PredNode *n = createPredNode(u.key);

				// Update predecessor array 
				sps.pred[adjacentNodeID]=n;

				// Insert v into the pq (Even if v is already there)
				ItemPQ v= createItem(adjacentNodeID, adjacents->weight);
				PQAdd(pq, v);
			}
			 
			adjacents = adjacents->next; 
		}
		
	}

	for (int i = 0; i < sps.numNodes; i++) {	
		// Change from INT_MAX to 0, 0 meaning no available path
		if(sps.dist[i] == INT_MAX) {
			sps.dist[i] = 0;
		}
	}

	PQFree(pq);

	return sps;
}

static ItemPQ createItem(Vertex n, int weight) {

	ItemPQ source;
	source.key = n;
	source.value= weight;
	return source;
}

// Create a Linked List Node for Predecessor Array
static PredNode *createPredNode(Vertex v) {

	PredNode *n = malloc(sizeof(PredNode));
	n->v = v;
	n->next= NULL;
	return n;
}

/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
void showShortestPaths(ShortestPaths sps) {

	printf("\nShortest path\n");

	int i = 0;
	while (i < sps.numNodes) {

		printf("Predecessor of %d of source %d is ", i, sps.src);
		PredNode *curr = sps.pred[i];
		while (curr != NULL) {

			printf("%d -> ", curr->v);
			curr = curr ->next;
		}
		printf("NULL\n");
		i++;
	}

}

/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps) {

	free(sps.dist);
	
	PredNode *tmp;
	int i = 0;
	while (i < sps.numNodes) {
		while (sps.pred[i] != NULL) {
			tmp = sps.pred[i];
			sps.pred[i] = sps.pred[i]->next;
			free(tmp);
		}
		i++;
		
	}
	
	free(sps.pred);
	

}

