// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
int numShortestPaths(ShortestPaths sps, PredNode *node, NodeValues nvs);
int updateBetweennessPerNode(ShortestPaths sps, PredNode *node, NodeValues nvs, int currNodeID);


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

			if (paths.dist[j] == 0 && j != paths.src){
				n--;
			}
			totalClosenessPerNode = totalClosenessPerNode + paths.dist[j];
			j++;
		}
		// printf("totalClosenessPerNode is %f\n", totalClosenessPerNode);
		// printf("n is %d\n", n);
		// printf("N is %d\n", N);

		// No isolated nodes
		if (totalClosenessPerNode ==0) {
			closeness.values[i] = 0.0;
		} else if (n == N){
			closeness.values[i] = (N-1)/totalClosenessPerNode;	
		} else {
			// printf("totalClosenessPerNode is %f\n", totalClosenessPerNode);
			// printf("n is %d\n", n);
			// printf("N is %d\n", N);
			//TYPE CASTING PLS
			closeness.values[i] = ((n-1.0)/(N-1.0))*((n-1.0)/totalClosenessPerNode);

		}

		// printf("%d: %f\n",i, closeness.values[i]);
		i++;
	}
	
	return closeness;
}

NodeValues betweennessCentrality(Graph g) {

	int numVertices = GraphNumVertices(g);
	NodeValues frequency = {0};
	frequency.numNodes = numVertices;
	frequency.values = calloc(numVertices, sizeof(double));

	NodeValues totalBetweenness = {0};
	totalBetweenness.numNodes = numVertices;
	totalBetweenness.values = calloc(numVertices, sizeof(double));
	

	// For every source
	for (int i = 0; i < numVertices ; i++) {
		// printf("\n------Source is %d -------\n", i);
		ShortestPaths paths = dijkstra(g, i);

		for (int j = 0; j < numVertices; j++){
			// for each destination
			// printf("Dest is %d\n", j);

			if (paths.pred[j] != NULL) {

				for (int k = 0; k < numVertices ; k++) {
					frequency.values[k] = 0;
				}
				int num = numShortestPaths(paths, paths.pred[j], frequency);

				updateBetweennessPerNode(paths, paths.pred[j], frequency, j);
				frequency.values[j] = 0;
				// printf("num of shortest paths from %d to %d is %d\n", i, j, num);

				// printf("-----frequency-----\n");
				// for (int k = 0; k < numVertices ; k++) {
				// 	printf("%d: %f\n",k, frequency.values[k]);
				// }

				// printf("------------\n");
				for (int k = 0; k < numVertices; k++) {
					totalBetweenness.values[k] = totalBetweenness.values[k] + frequency.values[k]/num;
				}

			}
			
		}
		
		// showShortestPaths(paths);
	}

	// printf("\n-----TOTAL -----\n");
	// for (int i = 0; i < numVertices ; i++) {
	// 	printf("%d: %f\n",i, totalBetweenness.values[i]);
	// }

	freeNodeValues(frequency);
	
	return totalBetweenness;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	
	int numVertices = GraphNumVertices(g);
	assert (numVertices > 2);

	NodeValues betweennessNormalised = {0};
	betweennessNormalised.numNodes = numVertices;
	betweennessNormalised.values = calloc(numVertices, sizeof(double));

	// printf("num vertices is %d\n", numVertices);
	double constant = 1.0/((numVertices - 1)*(numVertices - 2));
	// printf("constant is %f\n", constant);
	betweennessNormalised = betweennessCentrality(g);

	for (int k = 0; k < numVertices; k++) {
		betweennessNormalised.values[k]  = betweennessNormalised.values[k]*constant;
		
	}

	// for (int k = 0; k < numVertices; k++) {
	// 	printf("%d: %f\n", k, betweennessNormalised.values[k]);
			
	// }
	
	return betweennessNormalised;
}

void showNodeValues(NodeValues nvs) {

	for (int k = 0; k < nvs.numNodes; k++) {
		printf("%d: %f\n", k, nvs.values[k]);
			
	}

}

void freeNodeValues(NodeValues nvs) {
	free(nvs.values);
	// free(nvs);
}

int numShortestPaths(ShortestPaths sps, PredNode *node, NodeValues nvs)  {
	//printf("\n>>>>> %d Im being called\n", node->v);
	// int cur = q;
	int foundPath = 0;
	while (node != NULL) {
		// found a source
		if (node->v == sps.src) {
			foundPath = 1 ;	 
			//printf("Found a zero\n");
			// nvs.values[cur]++;
			//Update all recusion layers above
			
		
		} else {
			int i = node->v;
			//nvs.values[i] ++;
			//nvs.values[cur]++;
			//printf("-----------------------------------i is %d\n", i);
			//foundPath = foundPath + numShortestPaths(sps, sps.pred[i], nvs);
			foundPath = foundPath + numShortestPaths(sps, sps.pred[i], nvs);
			// nvs.values[cur]+= numShortestPaths(sps, sps.pred[i], nvs, i);
		}
		node = node->next;
	}
	return foundPath;
	
}

int updateBetweennessPerNode(ShortestPaths sps, PredNode *node, NodeValues nvs, int currNodeID)  {
	//printf("\n>>>>> %d Im being called\n", node->v);

	// q is the predecessor root, at the beginning of recrusion it represents the destination
	int cur = currNodeID;
	int found = 0;
	while (node != NULL) {
		// found a source
		if (node->v == sps.src) {
			//printf("Found a source\n");
			nvs.values[cur]++;
			found++;
			//Update all recursion layers above
			
		
		} else {
			int i = node->v;
			//nvs.values[i] ++;
			// nvs.values[cur]++;
			//printf("-----------------------------------i is %d\n", i);
			//foundPath = foundPath + numShortestPaths(sps, sps.pred[i], nvs);
			// printf("nvs.values[cur] is %f\n", nvs.values[cur]);
			nvs.values[cur] = nvs.values[cur]+ updateBetweennessPerNode(sps, sps.pred[i], nvs, i) ;

			PredNode *current = node;
			int totalNodes = 0;
			while (current != NULL) {
				totalNodes ++;
				current = current->next;
			}  

			if (totalNodes > nvs.values[cur]) {
				nvs.values[cur] = totalNodes ;
			}

			// printf("nvs.values[cur] is %f\n", nvs.values[cur]);
			// printf("found is %d\n", found);
			// updateBetweennessPerNode(sps, sps.pred[i], nvs, i);
		}
		node = node->next;
	}
	return found;
}

 




