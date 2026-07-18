#include <stdlib.h>
#include <stdio.h>
#include "merge.h"
#include "../../structures/structures.h"
#include "../../structures/structure_traversal/StructureTraversal.h"

//merging should merge branches
void MergeBranches(TreeNode *branchMT, TreeNode *branchBM)
{

    //MT -> merged to
    //BM -> Being merged
    // Ensure we do not attempt to merge NULL tree nodes
    if (branchMT == NULL || branchBM == NULL)
    {
        fprintf(stderr, "Error: Cant merge NULL tree nodes\n");
        return;
    }

    // Ensure both nodes actually contain valid branch stacks
    if (branchMT->data == NULL || branchBM->data == NULL)
    {
        fprintf(stderr, "Error: Cant merge empty BranchStacks\n");
        return;
    }


    //ensurnig that there is data to merge
    if (branchMT->data->arr->data==NULL || branchBM->data->arr->data==NULL)
    {
        fprintf(stderr, "Error : Cant merge empty data nodes\n");
        return;
    }


    // i was supposed to make sure it merges onto it as a commit there for the commit becomes the merge inside the node alowinf or data of that
    // commit to become the merge of the two branches (it shouldnt form a new branch)

    //the new capcity for the merge
    int capacity = branchMT->data->capacity + branchBM->data->capacity+1;

    // the branch stacks of each branch 
    BranchStack
        *stack_branchMT = branchMT->data,
        *stack_branchBM = branchBM->data;

    // Expands the existing block of contiguous memory to fit the new capacity
    Node *temp_arr = (Node *)realloc(stack_branchMT, capacity * sizeof(Node));

    if(temp_arr==NULL)
    {
        perror("Error : failed to reallocate memory to grow stack\n");
        return;
    }

    stack_branchMT->arr = temp_arr;
    

}