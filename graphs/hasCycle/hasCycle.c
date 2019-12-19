
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Stack.h"
// Use DFS concept to find a cycle in undirected graphs
// An undirected graph has a cycle if and only if 
// a depth-first search (DFS) finds an edge that points to an already-visited vertex (a back edge).
// Refer lec slides for details
// https://stackoverflow.com/questions/19113189/detecting-cycles-in-a-graph-using-dfs-2-different-approaches-and-whats-the-dif
bool recursive(Graph g, int v, int prevNode, int *visited) {	
	
	visited[v] = 1;	// visited the node - '1'
	for (int w = 0; w < GraphNumVertices(g); w++) {

		if(GraphIsAdjacent(g, v, w)) {	// if adjacent to the source
			if (w != prevNode && visited[w] == 1) {	// the only line that matters
				return true;	// got cycle
			
			// swap w and v; now v is the previous node/predecessor node and 
			// w is the source node for the next recursion
			}  else if (visited[w] == 0 && recursive(g, w, v, visited)== true)	{	
				return true;
			}
		}
	}
	return false;	// no cycle
}

bool hasCycle(Graph g) {
	int numNodes = GraphNumVertices(g);
	int visited[numNodes];


	for (int i =0; i < numNodes; i++) {
		memset(visited, 0, numNodes*sizeof(int)); 	// 0 means not yet visited
		if(recursive(g, i, i, visited) == true) {
			return true;
		}
	}	
	return false;
}

