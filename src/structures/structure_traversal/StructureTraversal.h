#ifndef STRUCUTURETRAVERSAL_H
#define STRUCUTURETRAVERSAL_H
#include "../structures.h"
#include "stdbool.h"


//--checking if a node has children
bool HasChild(TreeNode *root);

// --prototype functions--
void TraversePreOrder(TreeNode *root);
void TraverseInOrder(TreeNode *root);
void TraversePostOrder(TreeNode *root);


// --finding  a certain node based of its name--
TreeNode *Find(TreeNode *root, char *target);

#endif