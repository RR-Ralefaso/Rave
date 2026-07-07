#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>

// 1. Fixed TreeNode typedef
typedef struct TreeNode
{
    void *data;             // pointer to any data type
    struct TreeNode *left;  // left child
    struct TreeNode *right; // right child
} TreeNode;                 
// 2. Fixed BranchStack typedef
typedef struct BranchStack
{
    int *arr;     // pointer to dynamic array
    int top;      // index of the top element
    int capacity; // current maximum size
} BranchStack;    


// --- Function Prototypes ---

// Tree Functions
TreeNode *Init(void *data);

// Stack Functions 
BranchStack *InitStack(int capacity);
void Push(BranchStack *stack, int value);
int Pop(BranchStack *stack);
int IsEmpty(BranchStack *stack);

#endif