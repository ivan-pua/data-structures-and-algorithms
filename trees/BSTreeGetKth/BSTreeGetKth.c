
#include <stdlib.h>

#include "BSTree.h"
// Method 1 - Recursion using Ranking 
// https://stackoverflow.com/questions/2329171/find-kth-smallest-element-in-a-binary-search-tree-in-optimum-way?page=1&tab=votes#tab-top
int numNodes (BSTree t);
int BSTreeGetKth(BSTree t, int k) {

	if(t == NULL ){
		return 0;
	} else  {
		
		// Smallest number is at the left sub-tree; this is a feature of BS Tree
		int l = numNodes(t->left); 	
		
		// If the number of nodes at the Left subtree = key, then the key is the kth largest
		if(k == l) {				
			return t->value;
		
		// If k > num of nodes at left subtree, the key is at the RHS
		} else if (k > l) {
		
		// Deduct
			k = k -(l+1);
			return BSTreeGetKth(t->right, k);
		} else {
		
		// If k < num if nodes at the right subtree, the key is at the LHS
			return BSTreeGetKth(t->left, k);
		}
	}
}

// Calculate the number of nodes in the tree
int numNodes (BSTree t) {

	if (t == NULL){
		return 0;
	} else {
		return numNodes(t->left) + numNodes(t->right) +1;
	}
}


// Method 2 - In order Traversal
// https://leetcode.com/problems/kth-smallest-element-in-a-bst/discuss/63916/A-12ms-C-solution 
// https://www.geeksforgeeks.org/kth-largest-element-in-bst-when-modification-to-bst-is-not-allowed/ 
// #include <stdlib.h>
// #include <stdio.h>
// #include "BSTree.h"

// /* Inorder traversal to get to kth node. -1 indicates the function
// did not find the node in that subtree.*/
// int func(BSTree t, int k, int *index) {
// 	if (t != NULL) {
// 		int left = func(t->left, k, index);
// 		if (left != -1) return left;
// 		if (*index == k) return t->value;
// 		*index = *index + 1;
// 		int right = func(t->right, k, index);
// 		if (right != -1) return right;
// 	}
// 	return -1;
// }

// int BSTreeGetKth(BSTree t, int k) {
// 	int index = 0;
// 	return func(t, k, &index); // passed by reference 
// }
