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
    TreeNode *childL1 = CreateChild(root, "childL1");

    // Create a branch off of root (sibling to stan) using your branch function
    TreeNode *childR1 = CreateBranchBasedOfRoot(root, "ChildR1");

    // Create a grandchild under stan (using the correct, non-overwritten pointer)
    CreateChild(root, "Based_of_root");
    CreateChild(childL1, "childL1test"); //creating a child for childL1
    //checking if i can create another child for master
    CreateChild(root, "child3");

    printf("Actual Tree Traversal:\n");
    TraversePreOrder(root);

    // Free allocated memory
    FreeTree(root);

    end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}