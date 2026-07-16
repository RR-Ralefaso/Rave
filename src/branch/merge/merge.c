#include <stdlib.h>
#include <stdio.h>
#include "merge.h"
#include "../../structures/structures.h"
#include "../../structures/structure_traversal/StructureTraversal.h"

//merging should merge branches
void MergeBranches(TreeNode *branchMT, TreeNode *branchBM)
{
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

}