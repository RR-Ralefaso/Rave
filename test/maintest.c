#include <stdlib.h>
#include <stdio.h>
#include "../src/structures.h"
#include "../src/StructureTraversal.h"




int main(int argc, char const *argv[])
{
    TreeNode *node = Init(InitStack(0)); //inititating

    //checking if traversing works
    TraverseInOrder(node);


    //cleaning up
    free(node->data->arr);
    free(node->data);
    free(node);
    return 0;
}

