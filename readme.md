# RAVE

## About

- Creating an open-source version control system similar to Git.
- Built using C.
- Created by [github.com/RR-Ralefaso](https://github.com/RR-Ralefaso).

## difference from Git
 - it'll run on the assumption that conflicts are to ignored overwriting them
 - most commands will be force commands 
 - if a conflict is found it'll create a detached branch of that conflict not part of the branchs its on

## purpose for creation
 - for fun
 - allow others to add their own features theyd like
 - to improve my grasp of c and cpp

## Branch & Node Naming Convention
The system tracks repository ancestry using a path-based hierarchy (separated by `/`). This cleanly maps the exact genealogy of your VCS layout without string inflation or memory bloat:

- **Root Branch:** `master`
- **Sub-Branch:** `master/childL`
- **Nested Branch/Child:** `master/childL/childL2`

```text
[ root ] 
  │ 
  └── Identity: "master"
        │
        ├── (Left Child Pointer)
        │     ▼
        │   [ childL ] ───(Right Sibling Pointer)───► [ ChildR ]
        │     │                                       │
        │     │                                       └── Identity: "master/ChildR"
        │     ├── Identity: "master/childL"
        │     │
        │     └── (Left Child Pointer)
        │           ▼
        │         [ childL2 ]
        │           │
        │           └── Identity: "master/childL/childL2"

```

### to run (so far until i update it)

```
->assumming you in the Rave folder

1) running test :
        mkdir build
        gcc test/maintest.c src/structures/structures.c src/structures/structure_traversal/StructureTraversal.c src/branch/branches.c src/commits/commits.c -o build/testmain
        ./build/testmain 

2) Running main:
        mkdir build
        gcc main.c src/structures/structures.c src/structures/structure_traversal/StructureTraversal.c src/branch/branches.c -o -g main
        ./main

```

#### personal notes

```
    [x] work on naming logic for child nodes
    [x] created ways to commit 
    [] rethink the logic behind child nodes
    [] think about how one could create a child of a commit(stack)
    [] work on ensuring that names are unique so we can use em for commit identities eg master1  with master being the branch and 1 being the first commit


```
