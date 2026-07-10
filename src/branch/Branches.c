#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../structures/structure_traversal/StructureTraversal.h"
#include "branches.h"

/*
 - Adds a left child first if no children exist for the root node.
 - If a child exists, it adds it to the right (as a sibling).
 - If the node already has children and we want to create another branch dependent on the root,
   it becomes the child of the left child (always).
 - Child nodes will always have the name of the root node incorporated, even if attached to a sub-child.
*/

TreeNode *CreateChild(TreeNode *root, char *childname)
{
    // Ensure the parent root exists
    if (root == NULL)
    {
        printf("Error: The VCS hasn't been initialized\n");
        return NULL;
    }

    // Safety Check: Handle NULL or empty string inputs first
    if (childname == NULL || childname[0] == '\0')
    {
        printf("Error: Tried Creating a nameless Branch\n");
        return NULL;
    }

    // Initialize the new child node
    TreeNode *child = Init(InitStack(0), childname);
    if (child == NULL)
        return NULL;

    // Handle Identity/Naming Logic Safely using the main root's name
    char *parent_id = root->identity ? root->identity : "";

    // Clear any memory allocated for identity inside Init to prevent a leak
    if (child->identity != NULL)
    {
        free(child->identity);
        child->identity = NULL;
    }

    // Name format: childname_rootnameCHILD
    int total_len = strlen(childname) + 1 + strlen(parent_id) + strlen("CHILD") + 1;
    child->identity = (char *)malloc(total_len);
    if (child->identity == NULL)
    {
        return NULL;
    }

    strcpy(child->identity, childname);
    strcat(child->identity, "_");
    strcat(child->identity, parent_id);
    strcat(child->identity, "CHILD");

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

//-- MANAGING CREATING BRANCH BASED OFF THE ROOT

TreeNode *CreateBranchBasedOfRoot(TreeNode *root, char *branchbeingcreated)
{
    // Sanity check to protect repository state. If the pointer to the root is completely
    // empty, it implies the version control environment was never allocated in memory.
    if (root == NULL)
    {
        printf("Error: VCS system has not been initialised\n");
        return NULL;
    }

    // Inspect the root node to determine if any branch history or lineage already exists.
    // The HasChild function checks whether the root possesses an active left-child pointer.
    if (!HasChild(root))
    {
        // Lineage condition: The root node is completely isolated and has no sub-branches.
        // We establish a brand new, baseline tracking branch positioned directly beneath the root.
        printf("Created initial branch under root\n");
        return CreateChild(root, branchbeingcreated);
    }
    else
    {
        // Lineage condition: Parallel or historical branches already exist on this root.
        // To preserve a clear developmental hierarchy, any newly forced root-dependent branch
        // must drop down and attach as an offspring to the established leftmost child's lineage.
        printf("Root already has children. Appending branch to the left child's lineage.\n");

        // Execution handoff: We pass the root's left child into CreateChild. This guarantees
        // the node is inserted at the bottom of the leftmost branch's chain, while safely
        // adopting naming attributes downstream.
        return CreateChild(root->left, branchbeingcreated);
    }

    // Fallback safety return statement to satisfy compiler layout requirements in the
    // event that control flow unexpectedly escapes the primary conditional code blocks.
    return NULL;
}