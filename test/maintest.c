#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include "../src/structures.h"
#include "../src/StructureTraversal.h"

int main(int argc, char const *argv[])
{
    // clock_t for standard tick tracking
    clock_t start, end;

    start = clock();

    // ---CODE ---
    TreeNode *root = Init(InitStack(0), "master");

    CreateChild(root, "");

    printf("--- Traversal Output ---\n");
    TraverseInOrder(root);
    printf("------------------------\n");

    free(root->data->arr);
    free(root->data);
    free(root);
    // -------------------------

    end = clock();

    // Calculate time spent in microseconds
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;

    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}