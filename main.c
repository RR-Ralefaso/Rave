#include <stdio.h>
#include <stdlib.h>
#include "src/structures.h"



int main(int argc, char const *argv[])
{

    // Capture the return value so you don't lose your root node!
    TreeNode *root = Init(InitStack(0)); //initalising the version control system

    if (root == NULL || root->data == NULL) {
        printf("Failed to initialize the tree structure.\n");
        return EXIT_FAILURE;
    }

    // "Initial commit" verification
    printf("Tree root initialized successfully!\n");
    printf("Embedded stack capacity: %d\n", root->data->capacity);

    // --- Clean up memory before exiting ---
    free(root->data->arr); // Free the array inside the stack
    free(root->data);      // Free the stack structure itself
    free(root);           // Free the tree node

    return EXIT_SUCCESS;
}