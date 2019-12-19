#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

BSTree newNode(int val) {
	BSTree new = malloc(sizeof(struct BSTNode)); 
	// BSTree temp =  malloc(sizeof(BSTree)); THIS IS WRONG 
	// ^ We are mallocing a pointer (4 bytes) but the struct may be 12 bytes, so we are 'losing' some memory.
	// Always malloc(sizeof(struct structName))
	new->left = new->right = NULL;
	new->value = val;
	return new;
}

// Basic insert algo. Check lec slides for more info.
BSTree BSTreeInsert(BSTree t, int val) {
	
	// Base Case - if empty tree or already reached the leaf
	if (t == NULL) {
		return newNode(val);

	} else if (val > t->value) {
		t->right = BSTreeInsert(t->right, val);

	} else if (val < t->value) {
		t->left = BSTreeInsert(t->left, val);
	}
	// means we found the node which value == val
	return t;
}


// #include <stdio.h>
// #include <stdlib.h>

// #include "BSTree.h"

// static BSTree newBSTNode(int val);

// BSTree BSTreeInsert(BSTree t, int val) {
	
	
// 	// Base Case - if empty tree or already reached the leaf
// 	if(t == NULL) {
// 		BSTree node = newBSTNode(val);
// 		return node;
// 	}

// 	if (t->value == val) {
// 		return t;
// 	}

// 	// Recursive case
// 	if (val < t->value) {
// 		t->left= BSTreeInsert(t->left, val);
// 	} else  {
// 		t->right = BSTreeInsert(t->right, val);
// 	}

// 	return t;
// }

// static BSTree newBSTNode(int val) {

// 	BSTree temp =  malloc(sizeof(struct BSTNode)); 
// 	temp->value = val;
// 	temp->left = temp->right = NULL;
// 	return temp;
// }

