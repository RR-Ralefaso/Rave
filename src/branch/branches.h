#ifndef BRANCHES_H
#define BRANCHES_H

#include <stdlib.h>
#include "../structures/structures.h"




//--prototype functions
TreeNode *CreateBranchBasedOfRoot(TreeNode *root, char *branchbeingcreated);

// adding a childnode
TreeNode *CreateChild(TreeNode *root, char *childname);

#endif