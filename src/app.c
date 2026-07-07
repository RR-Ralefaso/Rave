#include <stdio.h>
#include <stdlib.h>
#include "structures.h"


int main(int argc, char const *argv[])
{
    //idk what the project is suposed to be 
    return 0;
}

TreeNode *Init(void *data)
{
    //  Allocate memory for the new node
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

    // Check if the memory allocation was successful
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Assign the data and initialize children to NULL
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    // Return the newly created node
    return newNode;
}



