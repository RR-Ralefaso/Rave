#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structures.h"

TreeNode *Init(BranchStack *stack, char *name)
{
    //Allocate memory for the new node
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

    // Check if memory allocation was successful
    if (newNode == NULL){
        perror("Error: Memory allocation failed for TreeNode.\n");
        return NULL;
    }

    newNode->data = stack;
    newNode->left = NULL;
    newNode->right = NULL;

    //Allocate separate memory for the string and copy it (Deep Copy)
    if (name != NULL){
        newNode->identity = (char *)malloc(strlen(name) + 1);
        if (newNode->identity != NULL){
            strcpy(newNode->identity, name); // Safely duplicate the string data
        }
    }
    else{
        newNode->identity = NULL;
    }
    //Return the newly created node
    return newNode;
}

//-initialising linked list-
Node *InitNode()
{
    Node *NewNode = (Node *)malloc(sizeof(Node));
    if (NewNode == NULL)
    {
        perror("Error : failed to allocate memory for Node\n");
        return NULL;
    }

    NewNode->data = NULL;
    NewNode->next = NULL;
    return NewNode;
}

// Memory cleanup function to accompany your Init function
void FreeTree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    //Recursively free the children first (going down)
    if (root->left != NULL)
    {
        FreeTree(root->left);
    }

    //Recursively free the siblings (going right)
    if (root->right != NULL)
    {
        FreeTree(root->right);
    }

    //Free the inner allocated memory for THIS node
    if (root->identity != NULL)
    {
        free(root->identity);
    }

    if (root->data != NULL)
    {
        // Free the internal array inside your BranchStack structure
        if (root->data->arr != NULL)
        {
            free(root->data->arr);
        }
        free(root->data); // Free the BranchStack itself
    }

    //Finally, free the node itself
    free(root);
}

BranchStack *InitStack(int capacity)
{
    BranchStack *stack = (BranchStack *)malloc(sizeof(BranchStack));
    if (stack == NULL)
    {
        perror("Error: Memory Allocation failed for the branch\n");
        return NULL;
    }
    // Allocates space for 'capacity' number of Node structs contiguously
    stack->arr = (Node *)malloc(capacity * sizeof(Node));
    if (stack->arr == NULL)
    {
        perror("Error : memory Allocation failed for stack array\n");
        free(stack);
        return NULL;
    }
    // Safely initialize the pre-allocated nodes inside the array
    for (int i = 0; i < capacity; i++)
    {
        stack->arr[i].data = NULL;
        stack->arr[i].next = NULL;
    }
    stack->capacity = capacity;
    stack->top = -1;
    stack->identity = 0;

    return stack;
}

Node *Pop(BranchStack *stack)
{
    // Checks for Stack Underflow (is the stack empty?)
    if (IsEmpty(stack))
    {
        perror("Error: Stack Underflow! Cannot pop from an empty stack.\n");
        // Return NULL to show failure (since the return type is a pointer)
        return NULL;
    }

    // Grab the ADDRESS of the Node at the top of the stack
    Node *poppedValue = &(stack->arr[stack->top]);

    // Move the top index down by one
    stack->top--;

    // Return the pointer to the node
    return poppedValue;
}

//--isempty 
int IsEmpty(BranchStack *stack)
{
    // If top is -1, the stack is empty (returns 1 for true, 0 for false)
    return (stack->top == -1);
}

//--Push
void Push(BranchStack *stack, void *data)
{
    // Checks if the stack pointer itself is valid before performing operations
    if (stack == NULL)
    {
        fprintf(stderr, "Error: No working stack available\n");
        return;
    }
    // Checks if the stack has reached its maximum allocated capacity limits
    if (stack->top >= stack->capacity - 1)
    {
        fprintf(stderr, "Stack Overflow! Cannot push.\n");
        return;
    }
    // Moves the top pointer tracking index up to the next available slot
    stack->top++;

    // Assigns the generic data pointer directly to the current top node container
    stack->arr[stack->top].data = data;

    // Sets the top node next pointer to NULL since it is now the last element
    stack->arr[stack->top].next = NULL;

    // Links the immediately preceding array node next pointer to this new node
    if (stack->top > 0)
    {
        stack->arr[stack->top - 1].next = &(stack->arr[stack->top]);
    }
}

//pushing into the linkedlist

//i honestly forgot how to do this so i had to check my cs notes
void AppendIntoStackArray(BranchStack *stack, void *data)
{
    // Checks if the pre-allocated array is full
    if (stack->top >= stack->capacity - 1)
    {
        printf("Stack Overflow: Cannot append, array capacity reached.\n");
        return;
    }

    // Moves to the next available slot in the pre-allocated array
    stack->top++;

    // 3Populate the data container already waiting for us in the array
    stack->arr[stack->top].data = data;
    stack->arr[stack->top].next = NULL; // It's now the last element, so its next is NULL

    // 4Link the PREVIOUS node in the array to this new node
    // (This turns your contiguous array slots into a functional linked list chain!)
    if (stack->top > 0)
    {
        stack->arr[stack->top - 1].next = &(stack->arr[stack->top]);
    }
}