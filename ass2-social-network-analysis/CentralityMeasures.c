// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
static double numShortestPaths(ShortestPaths sps, PredNode *node);
static double numerator(ShortestPaths s, int source, int dest, int target, bool isTarget);

// Returns an closeness centrality of the nodes in a graph 
// i.e. sum of length of shortest paths between node x and all other nodes
NodeValues closenessCentrality(Graph g) {
	
	int N = GraphNumVertices(g);
	NodeValues closeness = {0};
	closeness.numNodes = N;
	closeness.values = calloc(N, sizeof(double));

	int i = 0;
	while (i < N) {
		ShortestPaths paths = dijkstra(g, i);

		// Minus the source node
		int n = N;
		double totalClosenessPerNode = 0;
		int  j = 0;
		while (j < N) {

			// If the distance is 0, source node and node j are not connected
			if (paths.dist[j] == 0 && j != paths.src){
				// number of nodes that node 'u' can reach is decreased 
				n--;
			}
			totalClosenessPerNode = totalClosenessPerNode + paths.dist[j];
			j++;
		}

		if (totalClosenessPerNode == 0) { // Isolated nodes
			closeness.values[i] = 0.0;
		} else if (n == N){		// All nodes are reachable 
			closeness.values[i] = (N-1)/totalClosenessPerNode;	
		} else { 				// Not all nodes are reachable 
			closeness.values[i] = ((n-1.0)/(N-1.0))*((n-1.0)/totalClosenessPerNode);
		}

		i++;
	}
	
	return closeness;
}

/*
 * Returns an betweenness centrality of the nodes in a graph
 * i.e.  	total number of shortest paths that pass through target node v
 * 			---------------------------------------------------------------
 * 				total number of shortest paths from node i to node j 
 */
NodeValues betweennessCentrality(Graph g) {

	int numVertices = GraphNumVertices(g);

	NodeValues totalBetweenness = {0};
	totalBetweenness.numNodes = numVertices;
	totalBetweenness.values = calloc(numVertices, sizeof(double));
	
	// For every source, i
	for (int i = 0; i < numVertices ; i++) {

		ShortestPaths paths = dijkstra(g, i);

		// for each destination, j
		for (int j = 0; j < numVertices; j++){
			
			// The total number of shortest paths or 'denominator' of the equation
			double x = numShortestPaths(paths, paths.pred[j]);

			// If there are no shortest paths, skip this iteration
			if(x == 0) continue;

			// For every target, v
			for (int v = 0; v < numVertices; v++) {
				
				// conditions for calculating the betweenness
				if(j != i && v != i &&  v != j) {
					
					// Returns the number of times the target is found within the shortest paths
					// Also the numerator of the equation above
					double y = numerator (paths, i, j, v, false);	
					double quotient = y/x;

					totalBetweenness.values[v] += quotient;
				}
			}			
		}
	}
	
	return totalBetweenness;
}

// Returns an normalised betweenness centrality of the nodes in a graph
NodeValues betweennessCentralityNormalised(Graph g) {
	
	int numVertices = GraphNumVertices(g);
	assert (numVertices > 2);

	NodeValues betweennessNormalised = {0};
	betweennessNormalised.numNodes = numVertices;
	betweennessNormalised.values = calloc(numVertices, sizeof(double));

	// Constant formula from assignmnet specifications
	double constant = 1.0/((numVertices - 1)*(numVertices - 2));

	// Call betweennessCentrality function 
	betweennessNormalised = betweennessCentrality(g);

	for (int k = 0; k < numVertices; k++) {
		betweennessNormalised.values[k]  = betweennessNormalised.values[k]*constant;
		
	}

	return betweennessNormalised;
}

// Displays Node Values 
void showNodeValues(NodeValues nvs) {

	for (int k = 0; k < nvs.numNodes; k++) {
		printf("%d: %f\n", k, nvs.values[k]);
			
	}

}

// Frees memory from 'NodeValues' struct
void freeNodeValues(NodeValues nvs) {
	free(nvs.values);
	// free(nvs);
}

/**
 * Returns the number of shortest paths from the destination to the source node
 * Achieved by backtracking the predecessor array
 */
static double numShortestPaths(ShortestPaths sps, PredNode *node)  {

	double foundPath = 0;
	
	while (node != NULL) {

		// Base case - found the source node i.e. a shortest path is established
		if (node->v == sps.src) {
			foundPath = 1 ;	 
		
		// Continue backtracking
		} else {

			int i = node->v;
			foundPath = foundPath + numShortestPaths(sps, sps.pred[i]);
		}
		node = node->next;
	}
	return foundPath;
	
}

// Returns the number of times the target v is found among the shortest paths 
static double numerator(ShortestPaths s, int source, int dest, int target, bool isTarget)  {
	
	double count = 0;

	// Base Case - if the target or 'v' is found
	if(dest == target) {
		isTarget = true; // flag
	}

	// If arrive at the source and the target is found in this shortest path
	if(dest == source && isTarget == true) {
		count = 1;
	} 
	
	// Traverses the linked list in the predecessor array 
	// Backtracks from the destination node
	PredNode *curr = s.pred[dest];
	while (curr!= NULL) {

		count += numerator(s, source, curr->v, target, isTarget);

		curr=curr->next;
	}

	return count;
}

 




