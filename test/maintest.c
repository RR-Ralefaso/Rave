#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include "../src/structures/structures.h"
#include "../src/structures/structure_traversal/StructureTraversal.h"
#include "../src/branch/branches.h"

int main(int argc, char const *argv[])
{
    // clock_t for standard tick tracking
    clock_t start, end;

    start = clock();

    // ---CODE ---
    TreeNode *root = Init(InitStack(0), "master");
    

    CreateChild(root, "stan"); //testing creating child
    CreateBranchBasedOfRoot(root, "childtest"); //testing 

    printf("--- Traversal Output ---\n");
    TraverseInOrder(root);
    printf("------------------------\n");

    FreeNode(root);
    // -------------------------

    end = clock();

    // Calculate time spent in microseconds
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;

    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}