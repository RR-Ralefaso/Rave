#include <stdlib.h>
#include "../structures/structures.h"
#include <stdbool.h>
#include <ctype.h>

/**
 * Checks if the passed generic value points to a TreeNode container.
 * * @param value A pointer to a GenericContainer (passed as void*)
 * @return 1 if the underlying data is a TreeNode, 0 otherwise
 */
int isTreeNode(void *value)
{
    // Safety check: a NULL pointer cannot represent a type
    if (value == NULL)
        return 0;
    // Cast the generic void* back to our control structure (GenericContainer*)
    GenericContainer *container = (GenericContainer *)value;
    // Check if the type tag matches TYPE_TREE_NODE
    return (container->type == TYPE_TREE_NODE) ? 1 : 0;
}

/**
 * Checks if the passed generic value points to a BranchStack container.
 * * @param value A pointer to a GenericContainer (passed as void*)
 * @return 1 if the underlying data is a BranchStack, 0 otherwise
 */
int isStack(void *value)
{
    // Safety check
    if (value == NULL)
        return 0;
    // Cast to access the container metadata
    GenericContainer *container = (GenericContainer *)value;
    // Check if the type tag matches TYPE_BRANCH_STACK
    return (container->type == TYPE_BRANCH_STACK) ? 1 : 0;
}

/**
 * Checks if the passed generic value points to a Node container.
 * * @param value A pointer to a GenericContainer (passed as void*)
 * @return 1 if the underlying data is a Node, 0 otherwise
 */
int isNode(void *value)
{
    // Safety check
    if (value == NULL)
        return 0;
    // Cast to access the container metadata
    GenericContainer *container = (GenericContainer *)value;
    // Check if the type tag matches TYPE_NODE
    return (container->type == TYPE_NODE) ? 1 : 0;
}