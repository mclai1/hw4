#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node * root);

bool equalPaths(Node * root)
{
    // If the tree is empty and the paths are equal, return true
    if (root == NULL || height(root) > 0){
        return true;
    }
    return false;
}

int height(Node * root)
{
    // If the tree is empty, return 0
    if (root == NULL){
        return 0;
    }
    // If the node is a leaf, return 1
    else if (root->left == NULL && root->right == NULL){
        return 1;
    }
    // If the node does not have a left child, return the height of the right child + 1
    else if (root->left == NULL){
        return height(root->right) + 1;
    }
    // If the node does not have a right child, return the height of the left child + 1
    else if (root->right == NULL){
        return height(root->left) + 1;
    }
    // If the node has both children, find their heights
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    // If the heights are unequal previously, return -1
    if (leftHeight == -1 || rightHeight == -1){
        return -1;
    }
    // If the heights are equal, return the height of the left child + 1
    if (leftHeight == rightHeight){
        return leftHeight + 1;
    }
    // The heights unequal, return -1
    return -1;
}