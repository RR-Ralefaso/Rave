#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>

// linked list
typedef struct Node
{
    void *data; // Pointer to generic data
    struct Node *next;
} Node;

// branch
typedef struct BranchStack
{
    Node *arr; // pointer to dynamic array --check
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
    char* identity; //identity
} TreeNode;



// --- Function Prototypes ---

// Tree Functions
TreeNode *Init(BranchStack *stack,char* name);
// Stack Functions
BranchStack *InitStack(int capacity);
//pushes onto the stack
void Push(BranchStack *stack, void* data);

//pops top of stack
Node *Pop(BranchStack *stack);

// If top is -1, the stack is empty (returns 1 for true, 0 for false)
int IsEmpty(BranchStack *stack);

//cleans up memory
void FreeTree(TreeNode *root);

//initiating the linked list
Node *InitNode();

//pushing into the linked list
void AppendIntoNode(Node *head, void *data);

#endif