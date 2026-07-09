#include <stdlib.h>
#include <stdio.h>
#include "../src/structures.h"
#include "../src/StructureTraversal.h"




int main(int argc, char const *argv[])
{
    TreeNode *root = Init(InitStack(0),"master"); //inititating
    //trying to find a branch that doesnt exist
    TreeNode *childNE = Init(InitStack(0), "child"); //child that doesnt exist
    Find(root,"child"); // should return false as branch does not exist

    //checking if traversing works
    TraverseInOrder(root);


    //cleaning up
    free(root->data->arr);
    free(root->data);
    free(root);
    return 0;
}

