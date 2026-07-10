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
    if (root == NULL)
    {
        printf("Error: The VCS hasn't been initialized\n");
        return NULL;
    }

    // Check if the input name is valid
    if (childname == NULL || childname[0] == '\0')
    {
        printf("Error: Tried Creating a nameless Branch\n");
        return NULL;
    }

    // Allocate and initialize the new child structure
    TreeNode *child = Init(InitStack(0), childname);
    if (child == NULL)
        return NULL;

    // Get parent identity string or default to empty string
    char *parent_id = root->identity ? root->identity : "";

    // Free the temporary identity string allocated by Init to prevent memory leak
    if (child->identity != NULL)
    {
        free(child->identity);
        child->identity = NULL;
    }

    int total_len;
    // Generate hierarchical name path if parent identity exists
    if (strlen(parent_id) > 0)
    {
        // Compute memory size: parent string + '/' slash + child string + null terminator
        total_len = strlen(parent_id) + 1 + strlen(childname) + 1;
        child->identity = (char *)malloc(total_len);
        if (child->identity == NULL)
        {
            return NULL;
        }
        // Format string into parent/child structure
        snprintf(child->identity, total_len, "%s/%s", parent_id, childname);
    }
    else
    {
        // Direct allocation if parent name is empty
        total_len = strlen(childname) + 1;
        child->identity = (char *)malloc(total_len);
        if (child->identity == NULL)
        {
            return NULL;
        }
        snprintf(child->identity, total_len, "%s", childname);
    }

    // Insert as left child if parent has no children yet
    if (root->left == NULL)
    {
        root->left = child;
        return child;
    }

    // Traverse the sibling chain to find the last child
    TreeNode *current = root->left;
    while (current->right != NULL)
    {
        current = current->right;
    }

    // Attach new child to the end of the sibling chain
    current->right = child;
    return child;
}

//-- MANAGING CREATING BRANCH BASED OFF THE ROOT

/**
 * @brief Creates a new branch based on the root node's positioning.
 * * @note String Naming Convention:
 * Newly generated branches follow a strict suffix-based concatenation format:
 * Format: [childname]_[rootname]CHILD
 * * Example:
 * - Given a root named "main" and a new branch named "feature",
 * the resulting identifier will be saved as: "feature_mainCHILD"
 */
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
        printf("Root already has children...Appending %s to the left child's lineage.\n",branchbeingcreated);

        // Execution handoff: We pass the root's left child into CreateChild. This guarantees
        // the node is inserted at the bottom of the leftmost branch's chain, while safely
        // adopting naming attributes downstream.
        return CreateChild(root->left, branchbeingcreated);
    }

    // Fallback safety return statement to satisfy compiler layout requirements in the
    // event that control flow unexpectedly escapes the primary conditional code blocks.
    return NULL;
}