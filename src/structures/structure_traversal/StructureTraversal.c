//used to travers the structures created
#include <stdlib.h>
#include <stdio.h>
#include "../structures.h"
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
            "Node ID: %s\nstack ID : %d\n Stack Top Index : %d\n",
            root->identity, root->data->identity, root->data->top);
    }else("Node ID: %s, Stack is NULL\n", root->identity);

    TraverseInOrder(root->right);
}

//traversing NLR
void TraversePreOrder(TreeNode *root){ 
    if (root==NULL)
        return;

    if (root->data != NULL)
        printf("Visiting Node %s (Stack ID: %d)\n", root->identity, root->data->identity);

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
    printf("Finished processing children of Node %s\n", root->identity);
}


//---------------finding a specifc branch (node)----


bool Find(TreeNode *root, char *target){ //takes in a treenode and a string 
    if (root==NULL){
        fprintf(stderr,"target branch does not exist\n");
        return false;
    }

    if(root->identity  == target){
        printf("target branch found\n");
        return true;
    }
    //returns 1 if found and 0 ig not
    if ((Find(root->left, target)) != 0)
        return true;

    return Find(root->right, target);
}

//checks if a node has a child
bool HasChild(TreeNode *root)
{
    //checks if the root exists if not it should return a warning and false
    if (root==NULL)
    {
        fprintf(stderr,"Error : Parent Node doesnt exist");
        return false;
    }

    // if root has a left or right child it returns true
    if ((root->right != NULL) || (root->left != NULL))
        return true;

}