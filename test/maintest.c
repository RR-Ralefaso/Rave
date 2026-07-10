#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../src/structures/structures.h"
#include "../src/structures/structure_traversal/StructureTraversal.h"
#include "../src/branch/branches.h"

int main(int argc, char const *argv[])
{
    clock_t start, end;
    start = clock();

    // Initialize root node
    TreeNode *root = Init(InitStack(0), "master");
    if (root == NULL)
    {
        printf("Error: Root initialization failed.\n");
        return 1;
    }

    // Create a first child under master
    TreeNode *stan = CreateChild(root, "stan");

    // Create a branch off of root (sibling to stan) using your branch function
    TreeNode *childtest_node = CreateBranchBasedOfRoot(root, "childtest");

    // Create a grandchild under stan (using the correct, non-overwritten pointer)
    CreateChild(stan, "stanmanson");

    printf("Actual Tree Traversal:\n");
    TraverseInOrder(root);

    // Free allocated memory
    FreeTree(root);

    end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}