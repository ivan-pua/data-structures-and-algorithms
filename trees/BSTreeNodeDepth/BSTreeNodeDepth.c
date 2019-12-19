
#include <stdlib.h>

#include "BSTree.h"

int BSTreeNodeDepth(BSTree t, int key) {
	
	if (t == NULL) {
		return -1;

	} else if (key < t->value){
		int leftDepth = BSTreeNodeDepth(t->left, key);
		if (leftDepth == -1) {
			return -1;
		} else {
			return leftDepth +1;
		}
	} else if (key > t->value) {
		int rightDepth = BSTreeNodeDepth(t->right, key);
		if (rightDepth == -1) {
			return -1;
		} else {
			return rightDepth +1;
		}

	} else {
		// We have encountered the node where t->value == key
		return 0;
	}
}

