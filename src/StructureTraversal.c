//used to travers the structures created
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "StructureTraversal.h"
#include <stdbool.h>



//        ------Transversing algorithms------------
// traversing  LNR

void TraverseInOrder(TreeNode *root){
    if (root==NULL)
        return;
    

    //go through the left child
    TraverseInOrder(root->left);

    // Visit the current node (Accessing the BranchStack data)
    if(root->data!=NULL){
        printf(
            "Node ID: %d\nstack ID : %d\n Stack Top Index : %d\n",
            root->identity, root->data->identity, root->data->top);
    }else("Node ID: %d, Stack is NULL\n", root->identity);

    TraverseInOrder(root->right);
}

//traversing NLR
void TraversePreOrder(TreeNode *root){ 
    if (root==NULL)
        return;

    if (root->data != NULL)
        printf("Visiting Node %d (Stack ID: %d)\n", root->identity, root->data->identity);

    TraversePreOrder(root->left);
    TraversePreOrder(root->right);
}

//traversing LRN
void TraversePostOrder(TreeNode *root)
{
    if (root == NULL)
        return;

    TraversePostOrder(root->left);
    TraversePostOrder(root->right);

    // Visit the root last (e.g., Useful for freeing memory)
    printf("Finished processing children of Node %d\n", root->identity);
}


//---------------finding a specifc branch (node)----


TreeNode *Find(TreeNode *root, TreeNode *target){
    if (root==NULL){
        printf("target branch does not exist");
        return NULL;
    }

    if(root->identity  == target->identity){
        printf("target branch found");
        return root;
    }

    TreeNode *FoundInLeft = Find(root->left, target);
    if (FoundInLeft != NULL)
        return FoundInLeft;

    return Find(root->right, target);
}
