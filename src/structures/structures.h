#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>

// linked list
typedef struct Node
{
    void *data; // Pointer to generic data
    //char *identity; //added identity that way a commit has an identity allowing modification and viewing
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

typedef enum
{
    TYPE_UNKNOWN,      // Default/fallback state to catch uninitialized containers
    TYPE_NODE,         // Identifies the underlying data as a 'Node' struct
    TYPE_BRANCH_STACK, // Identifies the underlying data as a 'BranchStack' struct
    TYPE_TREE_NODE     // Identifies the underlying data as a 'TreeNode' struct
} DataType;

/**
 * GENERIC CONTAINER 
 * Instead of passing raw, anonymous 'void *' pointers directly through your
 * system, you wrap them in this 'GenericContainer'.
 * * This creates a unified "envelope" that pairs the actual data pointer with
 * a metadata tag (the enum above) so functions can safely inspect and identify
 * what they are working with before casting or dereferencing.
 */
typedef struct
{
    DataType type; // The "tag" or "ID" telling us what kind of struct 'ptr' targets
    void *ptr;     // A generic pointer holding the memory address of the actual struct
} GenericContainer;

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