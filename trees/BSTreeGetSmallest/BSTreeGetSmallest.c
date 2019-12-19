
#include "BSTree.h"

#include <stdlib.h>

BSTree BSTreeGetSmallest(BSTree t) {
	// Empty Tree
	if( t == NULL) return NULL;

	// The smallest number is at the leftmost node of the tree
	if (t->left == NULL) {
		return t;
	} else {
		t = BSTreeGetSmallest(t->left);
	}
	return t;
}

