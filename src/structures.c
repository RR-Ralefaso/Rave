#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

TreeNode *Init(BranchStack *stack)
{
    // 1. Allocate memory for the new node
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

    // 2. Check if memory allocation was successful
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed for TreeNode.\n");
        return NULL;
    }

    newNode->data = stack; 
    newNode->left = NULL;
    newNode->right = NULL;

    // 4. Return the newly created node
    return newNode;
}

BranchStack *InitStack(int capacity){
    //allocate the size of the stack
    BranchStack *stack = (BranchStack *)malloc(sizeof(BranchStack));

    if(stack==NULL){
        printf("Error: Memory Allocation failed for the branch\n");
        return NULL;
    }

    stack->arr = (int *)malloc(capacity * sizeof(int));
    if (stack->arr == NULL){

     printf("Error : memory Allocation failed for stack array\n");
    free(stack);
    return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;

    return stack;
}

int Pop(BranchStack *stack)
{
    // hecks for Stack Underflow (is the stack empty?)
    if (IsEmpty(stack))
    {
        printf("Error: Stack Underflow! Cannot pop from an empty stack.\n");
        // Return a  value (like -1) to show failure.
        return -1;
    }

    // Grab the value at the top of the stack
    int poppedValue = stack->arr[stack->top];

    //Move the top index down by one
    stack->top--;

    //Return the value
    return poppedValue;
}


//--isempty 
int IsEmpty(BranchStack *stack)
{
    // If top is -1, the stack is empty (returns 1 for true, 0 for false)
    return (stack->top == -1);
}