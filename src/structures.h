#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>

// branch
typedef struct BranchStack
{
    int *arr;     // pointer to dynamic array
    int top;      // index of the top element
    int capacity; // current maximum size
    int identity;  // the identity 
} BranchStack;

//  TreeNode
typedef struct TreeNode
{
    BranchStack *data;             // pointer to any data type
    struct TreeNode *left;  // left child
    struct TreeNode *right; // right child
    int identity; //identity
} TreeNode;




// --- Function Prototypes ---

// Tree Functions
TreeNode *Init(BranchStack *stack);
// Stack Functions 
BranchStack *InitStack(int capacity);
void Push(BranchStack *stack, BranchStack* value);
int Pop(BranchStack *stack);
int IsEmpty(BranchStack *stack);

#endif