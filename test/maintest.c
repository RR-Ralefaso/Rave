#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../src/structures/structures.h"
#include "../src/structures/structure_traversal/StructureTraversal.h"
#include "../src/branch/branches.h"

int main(int argc, char const *argv[])
{
    // Track execution time
    clock_t start, end;
    start = clock();

    printf("==================================================\n");
    printf("         VCS TREE STRUCTURE VERIFICATION          \n");
    printf("==================================================\n\n");

    // Initialize Root
    printf("Initializing 'master' root...\n");
    TreeNode *root = Init(InitStack(0), "master");
    if (!root)
    {
        printf("FAIL: Root initialization failed.\n");
        return 1;
    }

    // Create Left Child
    printf("Creating first-generation child 'stan'...\n");
    TreeNode *stan = CreateChild(root, "stan");

    // Create Sibling (Right Child approach)
    printf("Creating 'childtest' branch off master root...\n");
    TreeNode *childtest_node = CreateBranchBasedOfRoot(root, "childtest");

    // Create Grandchild (Under the original 'stan' pointer)
    printf("Creating second-generation grandchild 'stanmanson' under 'stan'...\n\n");
    CreateChild(stan, "stanmanson");

    // Visual map to manually compare against traversal pattern
    printf("--- EXPECTED HIERARCHY ---\n");
    printf("master\n");
    printf("├── stan (Child of master)\n");
    printf("│   └── stanmanson (Child of stan)\n");
    printf("└── childtest (Sibling of stan)\n\n");

    printf("--- ACTUAL TRAVERSAL OUTPUT ---\n");
    TraverseInOrder(root);
    printf("-------------------------------\n\n");

    // Cleanup memory
    printf("Cleaning up tree memory...\n");
    FreeTree(root);
    printf("Memory freed successfully.\n\n");

    // Stop clock and calculate execution time
    end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;

    printf("==================================================\n");
    printf("Execution Speed: %.2f microseconds (us)\n", microseconds);
    printf("==================================================\n");

    return 0;
}