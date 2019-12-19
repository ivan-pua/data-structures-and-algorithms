
#include <stdlib.h>

#include "tree.h"
static int max(int left, int right);
int TreeHeight(Tree t) {

    // Base Case - if tree is empty OR if reached at the end of the tree (after the leaves)
    int maxHeight;
    if (t == NULL) {
        return -1;
    } else {
        int leftHeight = TreeHeight(t->left) + 1;
        int rightHeight = TreeHeight(t->right) + 1;
        maxHeight = max(leftHeight, rightHeight);
    }
    
    
    return maxHeight;
}

static int max(int left, int right) {

 /** 
    if (left > right) {
        return left;
    } else {
        return right;
    }
**/
    // Simplified with a line
    return left > right ? left : right;
}

