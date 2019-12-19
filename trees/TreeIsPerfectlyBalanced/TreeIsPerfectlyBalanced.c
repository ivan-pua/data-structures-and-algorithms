
#include <stdlib.h>
#include <math.h>

#include "tree.h"
// A tree is perfectly balanced if, for every node, 
// the difference in SIZE between its left and right subtrees does not exceed 1.
// NOT HEIGHT

/* Counts the number of nodes in left and right subtrees. Returns -1 for each
subtree if the difference between the two node counts are more than 1, returns
the node count otherwise.
*/

// Computes the number of nodes 
static int numNodes (Tree t) {

	if (t == NULL) {
		return 0;
	} else {
		return 1 + numNodes(t->left) +numNodes(t->right);
	}
}

static int check(Tree t) {
	
	if (t == NULL) return 1;	// a empty tree is always balanced - return true
	
	int leftSize = numNodes(t->left);
	int rightSize = numNodes(t->right);

	if (abs(leftSize-rightSize) > 1 || check(t->left) == -1 || check(t->right) == -1){
		return -1;	// means not perfectly balanced anymore, keeps returning -1
	}
	
	return 1;
}

bool TreeIsPerfectlyBalanced(Tree t) {
	if (check(t) != -1) {
		return true;
	} else {
		return false;
	}
}