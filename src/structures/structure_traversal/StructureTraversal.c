#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../structures.h"
#include "StructureTraversal.h"

// recursive helper to build out the tree lines and branches
static void VisualiseTreeHelper(TreeNode *root, char *prefix, bool isLeft)
{
    if (root == NULL)
        return;

    // print current node info
    printf("%s", prefix);
    printf("%s", isLeft ? "├── " : "└── ");
    printf("[%s]", root->identity ? root->identity : "NULL");

    if (root->data != NULL)
    {
        printf(" (Stack ID: %d, Top: %d)\n", root->data->identity, root->data->top);
    }
    else
    {
        printf(" (Stack: NULL)\n");
    }

    // append proper branch styling based on left/right position
    char newPrefix[512];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeft ? "│   " : "    ");

    // recurse through children
    if (root->left || root->right)
    {
        if (root->left)
        {
            VisualiseTreeHelper(root->left, newPrefix, root->right != NULL);
        }
        else
        {
            // mark missing left child if right child exists
            printf("%s├── (L: NULL)\n", newPrefix);
        }

        if (root->right)
        {
            VisualiseTreeHelper(root->right, newPrefix, false);
        }
    }
}

// main call to print the full tree visually
void VisualiseTree(TreeNode *root)
{
    if (root == NULL)
    {
        printf("(Tree is empty)\n");
        return;
    }

    printf("\n=== Tree Visualization ===\n");
    printf("[%s]", root->identity ? root->identity : "ROOT");
    if (root->data != NULL)
    {
        printf(" (Stack ID: %d, Top: %d)\n", root->data->identity, root->data->top);
    }
    else
    {
        printf(" (Stack: NULL)\n");
    }

    char prefix[512] = "";
    if (root->left)
    {
        VisualiseTreeHelper(root->left, prefix, root->right != NULL);
    }
    if (root->right)
    {
        VisualiseTreeHelper(root->right, prefix, false);
    }
    printf("==========================\n\n");
}

// ------Traversing algorithms------------

// LNR traversal
void TraverseInOrder(TreeNode *root)
{
    if (root == NULL)
        return;

    TraverseInOrder(root->left);

    if (root->data != NULL)
    {
        printf("Node ID: %s\nStack ID: %d\nStack Top Index: %d\n",
               root->identity, root->data->identity, root->data->top);
    }
    else
    {
        printf("Node ID: %s, Stack is NULL\n", root->identity);
    }

    TraverseInOrder(root->right);
}

// NLR traversal
void TraversePreOrder(TreeNode *root)
{
    if (root == NULL)
        return;

    if (root->data != NULL)
        printf("Visiting Node %s (Stack ID: %d)\n", root->identity, root->data->identity);

    TraversePreOrder(root->left);
    TraversePreOrder(root->right);
}

// LRN traversal
void TraversePostOrder(TreeNode *root)
{
    if (root == NULL)
        return;

    TraversePostOrder(root->left);
    TraversePostOrder(root->right);

    printf("Finished processing children of Node %s\n", root->identity);
}

// ---------------finding a specific branch (node)----

bool Find(TreeNode *root, char *target)
{
    if (root == NULL || target == NULL)
    {
        return false;
    }

    // strcmp fix for proper string checking
    if (root->identity != NULL && strcmp(root->identity, target) == 0)
    {
        printf("Target branch found: %s\n", target);
        return true;
    }

    if (Find(root->left, target))
        return true;

    return Find(root->right, target);
}

// checks if node has at least one child
bool HasChild(TreeNode *root)
{
    if (root == NULL)
    {
        fprintf(stderr, "Error: Parent Node doesn't exist\n");
        return false;
    }

    return (root->left != NULL || root->right != NULL);
}