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

BranchStack *InitStack(int capacity){
    //allocate the size of the stack
    BranchStack *stack = (BranchStack *)malloc(sizeof(BranchStack));

    if(stack==NULL){
        perror("Error: Memory Allocation failed for the branch\n");
        return NULL;
    }

    stack->arr = (Node *)malloc(capacity * sizeof(Node));
    if (stack->arr == NULL){

        perror("Error : memory Allocation failed for stack array\n");
        free(stack);
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->identity = 0; //the identity of the stack

    return stack;
}

// TODO : correct pop

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
void push(BranchStack *stack, Node* value)
{
    // if the stack is full
    if (stack->top >= stack->capacity - 1)
    {
        perror("Stack Overflow! Cannot push \n"); // check
        return ;
    }

    //Increment top and insert the integer
    stack->top++;
    stack->arr[stack->top] = *value; //check
}


//-initialising linked list-
Node *InitNode()
{
    Node *NewNode = (Node *)malloc(sizeof(Node));
    if (NewNode==NULL)
    {
        perror("Error : failed to allocate memory for Node\n");
        return NULL;
    }

    NewNode->data = NULL;
    NewNode->next = NULL;
    return NewNode;
}

//pushing into the linkedlist
void AppendIntoNode(Node **head, void *data)
{
    // initialize the new node
    Node *NewNode = InitNode();
    if (NewNode == NULL)
    {
        perror("Error : Memory Allocation Failed\n");
        return;
    }
    NewNode->data = data;
    // NewNode->next is already set to NULL by InitNode()
    // If the list is completely empty, make this the head node
    if (*head == NULL)
    {
        *head = NewNode;
        return;
    }
    //Otherwise, traverse to the end of the list
    Node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Link the old last node to our new node
    current->next = NewNode;
}