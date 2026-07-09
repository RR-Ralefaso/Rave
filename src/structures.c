#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "structures.h"

TreeNode *Init(BranchStack *stack, char *name)
{
    //Allocate memory for the new node
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

    // Check if memory allocation was successful
    if (newNode == NULL){
        printf("Error: Memory allocation failed for TreeNode.\n");
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
    stack->identity = 0; //the identity of the stack

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



//--Push
void push(BranchStack *stack, int value)
{
    // if the stack is full
    if (stack->top >= stack->capacity - 1)
    {
        printf("Stack Overflow! Cannot push %d\n", value);
        return;
    }

    //Increment top and insert the integer
    stack->top++;
    stack->arr[stack->top] = value;
}



/*
 - adds a left child fist if no children exist for the root node 
 - if a chld exists it addes it to the right
 - if the node already has children and we want to create another branch dependent on the root it becomes the children of the left child (always)
 - child nodes will have the name of the parent node
*/

TreeNode *CreateChild(TreeNode *root, char *childname)
{
    //Ensure the parent root exists
    if (root == NULL)
    {
        printf("Error: The VCS hasn't been initialized\n");
        return NULL;
    }

    // Initialize the new child node
    TreeNode *child = Init(InitStack(0), childname);
    if (child == NULL)
        return NULL;

    // Handle Identity/Naming Logic Safely
    char *parent_id = root->identity ? root->identity : "";

    // Clear any memory allocated for identity inside Init to prevent a leak
    if (child->identity != NULL)
    {
        free(child->identity);
        child->identity = NULL;
    }

    // string emptiness check using childname[0]
    if (childname[0] == '\0'){
        printf(" Error : Tried Creating a nameless Branch\n");
        return NULL;
    }

    if (childname == NULL)
    {
        // Name format: parentnameCHILD
        int total_len = strlen(parent_id) + strlen("CHILD") + 1;
        child->identity = (char *)malloc(total_len);
        strcpy(child->identity, parent_id);
        strcat(child->identity, "CHILD");
    }
    else
    {
            // Name format: childname_parentnameCHILD
        int total_len = strlen(childname) + 1 + strlen(parent_id) + strlen("CHILD") + 1;
        child->identity = (char *)malloc(total_len);

        strcpy(child->identity, childname);
        strcat(child->identity, "_");
        strcat(child->identity, parent_id);
        strcat(child->identity, "CHILD");    
   }  

    //Insert into the Tree (Left-Child, Right-Sibling approach)
    if (root->left == NULL)
    {
        root->left = child;
        return child;
    }

    // Navigate through right siblings to find the end of the children list
    TreeNode *current = root->left;
    while (current->right != NULL)
    {
        current = current->right;
    }

    current->right = child;
    return child;
}