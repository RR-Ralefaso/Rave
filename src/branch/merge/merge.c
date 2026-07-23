#include <stdlib.h>
#include <stdio.h>
#include "merge.h"
#include "../../structures/structures.h"
#include "../../commits/commits.h"
#include "../../structures/structure_traversal/StructureTraversal.h"

//merging should merge branches

/*
    merge isnt created correctly -fix

*/
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
    Commit(branchMT->data, branchBM);
}