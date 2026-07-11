#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../src/structures/structures.h"
#include "../src/structures/structure_traversal/StructureTraversal.h"
#include "../src/branch/branches.h"
#include "../src/commits/commits.h"

int main(int argc, char const *argv[])
{
    clock_t start, end;
    start = clock();

    // Initialize root node
    TreeNode *root = Init(InitStack(1),"master");
    if (root == NULL)
    {
        printf("Error: Root initialization failed.\n");
        return 1;
    }

    // // Create a first child under master
    // TreeNode *childL1 = CreateChild(root, "childL1");

    // // Create a branch off of root (sibling to stan) using your branch function
    // TreeNode *childR1 = CreateBranchBasedOfRoot(root, "ChildR1");

    // // Create a grandchild under stan (using the correct, non-overwritten pointer)
    // CreateChild(root, "Based_of_root");
    // CreateChild(childL1, "childL1test"); //creating a child for childL1
    // //checking if i can create another child for master
    
    //testing actually commiting
    /*
        so far we have nothing but the main branch that as a branch in it.
        theres nothing in the branch
        so im expecting to be like :

        (main) -inside main -> (stack with nothing in it) -inside the frst layer->(nothing in the node)
        */

    char data[] = "I AM GREATNESS"; //data

    Commit(root->data, &data);


    printf("Actual Tree Traversal:\n");


    TraversePreOrder(root);

    printf("\nthe first commit : \n");
    printf("    branch name : %s\n", root->identity);
    printf("    layer identity : %d\n", root->data->identity);
    printf("    Data committed : %s\n", (char *)root->data->arr[root->data->top].data);
    printf("I NEED TO SLEEP ZEHAHAHAHAHA.. \n");

    // Free allocated memory
    FreeTree(root);

    end = clock();
    double microseconds = ((double)(end - start) / CLOCKS_PER_SEC) * 1e6;
    printf("\nExecution Speed: %.2f microseconds (us)\n", microseconds);

    return 0;
}