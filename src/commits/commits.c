/*
will be used making commits , deleting commit , merging commits and so on
*/

#include <stdlib.h>
#include <stdio.h>
#include "../structures/structures.h"
#include "commits.h"




// function that commits into the branch stack
void Commit(BranchStack *stack, void *data)
{
    // Checks if the stack pointer itself is valid before performing operations
    if (stack == NULL)
    {
        fprintf(stderr, "Error: No working stack available\n");
        return;
    }
    // Ensure there is actual data payload to be added to the stack
    if (data == NULL)
    {
        printf("Nothing to Commit\n");
        return;
    }
    // Calculates the target capacity size needed to hold one additional node
    int new_capacity = stack->capacity + 1;

    // Expands the existing block of contiguous memory to fit the new capacity
    Node *temp_arr = (Node *)realloc(stack->arr, new_capacity * sizeof(Node));

    // Handle allocation failure safely without losing the original array memory
    if (temp_arr == NULL)
    {
        perror("Error: Failed to reallocate memory to grow the stack\n");
        return;
    }
    // Update the stack with the newly reallocated array pointer
    stack->arr = temp_arr;
    // Save the newly increased capacity counter back to the stack structure
    stack->capacity = new_capacity;
    // Delegate the actual insertion and pointer linking to the push function
    Push(stack, data);
}