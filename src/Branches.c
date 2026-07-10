#include <stdlib.h>
#include <stdio.h>
#include <StructureTraversal.h>
#include <Branches.h>
/*
 - adds a left child fist if no children exist for the root node
 - if a chld exists it addes it to the right
 - if the node already has children and we want to create another branch dependent on the root it becomes the children of the left child (always)
 - child nodes will have the name of the parent node
*/

TreeNode *CreateChild(TreeNode *root, char *childname)
{
    // Ensure the parent root exists
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
    if (childname[0] == '\0')
    {
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

    // Insert into the Tree (Left-Child, Right-Sibling approach)
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



//--MANAGING CREATING BRANCH BASED OF THE ROOT

TreeNode * CreateBranchBasedOfRoot(TreeNode *root , char *branchbeingcreated){

    //checks if the parent node exists
    if(root==NULL){
        printf("Error : VSC system has not been initialised\n");
        return NULL;
    }

    switch(HasChild(root))
    {
        //if no child itll create it as a child branch 
        case 0 :
            return CreateChild(root, branchbeingcreated);
            break;
        case 1 :
            break;

    }

}