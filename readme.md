# RAVE

## About

- **Overview:** An open-source, experimental Version Control System (VCS) built from the ground up.
- **Language:** C
- **Author:** [RR-Ralefaso(polaris)](https://github.com/RR-Ralefaso)

---

## Key Differences from Git

- **Aggressive Overwrites:** Operates on the assumption that file conflicts can be automatically ignored by default via overwriting.
- **Force-First Commands:** Built with a "force by default" mindset rather than stopping execution on safety checks.
- **Automatic Conflict Isolation:** When a conflict occurs during an operation, instead of halting, RAVE isolates the conflicting state into a new **detached branch**.

---

## Purpose & Goals

- **Learning:** Deepen understanding of low-level systems programming, memory management, and tree structures in C.
- **Customization:** Provide an extensible base where developers can easily experiment with and add non-traditional VCS features.
- **Fun:** Build a functional core tool from scratch for the joy of building.

---

# Data Persistence & Storage Architecture

RAVE uses a **Content-Addressable Storage (CAS)** model inside a hidden `.rave/` directory. Files are identified by their byte content rather than their file paths, maximizing deduplication and data integrity.

```text
.rave/
├── objects/
│   ├── 4b/
│   │   └── 825dc642...
│   └── 9f/
│       └── 110a2bc1...
├── commits/
├── refs/
│   └── heads/
│       └── master
└── index
```

## 1. File Storage Pipeline

1. **Hashing**
   - Compute a SHA-256 (or SHA-1) checksum of the incoming file buffer.

2. **Compression**
   - Compress raw byte streams using zlib/deflate.

3. **Writing Object**
   - Save the file to:

     ```text
     .rave/objects/<first_2_chars_of_hash>/<rest_of_hash>
     ```

### Benefits

- Identical file contents are only stored once regardless of filename or branch.
- Maximizes deduplication while maintaining integrity.

---

## 2. Version Snapshot Objects

### Blobs

Compressed byte arrays representing file contents.

### Trees

Directory manifests mapping:

- Filenames
- Object hashes
- Permissions

### Commits

Metadata containing:

- Tree Object Hash (root snapshot)
- Parent Commit Hash
- Hierarchical Identity (e.g. `master1`, `master/childL2`)
- Author
- Timestamp (`time_t`)

---

# Implementation Plan & Code Layout

## Target Directory Structure

```text
RAVE/
├── include/
│   ├── storage/
│   │   ├── hash.h
│   │   ├── compress.h
│   │   └── object.h
│   ├── structures/
│   │   ├── branch.h
│   │   └── commit.h
│   └── utils/
│       └── file_io.h
│
├── src/
│   ├── storage/
│   │   ├── hash.c
│   │   ├── compress.c
│   │   └── object.c
│   ├── structures/
│   │   ├── structures.c
│   │   └── structure_traversal/
│   │       └── StructureTraversal.c
│   ├── branch/
│   │   ├── branches.c
│   │   └── merge/
│   │       └── merge.c
│   └── commits/
│       └── commits.c
│
├── test/
│   └── maintest.c
└── main.c
```

---

# Implementation Execution Steps

## Step 1: Low-Level Storage Layer

**Location**

```text
src/storage/hash.c
src/storage/compress.c
```

**Tasks**

- Build:

```c
hash_buffer(const unsigned char *input, size_t len, char *out_hash)
```

- Build:

```c
compress_buffer(
    const unsigned char *src,
    size_t src_len,
    unsigned char **dest,
    size_t *dest_len
)
```

using zlib.

---

## Step 2: Object Store Manager

**Location**

```text
src/storage/object.c
```

### Implement

```c
write_object(const unsigned char *data, size_t len, const char *type)
```

Responsibilities:

- Prepend header (`blob 1024\0`)
- Hash payload
- Create `.rave/objects/ab/`
- Compress payload
- Write object

Also implement:

```c
read_object(const char *hash)
```

---

## Step 3: Staging Area & Tree Construction

**Location**

```text
src/commits/commits.c
src/storage/object.c
```

### Tasks

- Store staged files in `.rave/index`
- Build Tree objects
- Hash/store Tree
- Link Tree to Commit

---

## Step 4: Wire to CLI & Branch Hierarchy

**Location**

```text
main.c
src/branch/branches.c
```

### Tasks

- Associate commits with branch identities.
- Update:

```text
.rave/refs/heads/<branch_name>
```

with the latest commit hash.

---

# Architecture: Branch & Node Naming

RAVE tracks ancestry using hierarchical path-based names.

Examples:

- `master`
- `master/childL`
- `master/childL/childL2`

```text
[ Root ]
  │
  └── Identity: "master"
        │
        ├── (Left Child Pointer)
        │     ▼
        │   [ childL ] ───(Right Sibling Pointer)──► [ ChildR ]
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

---

# Build & Run Instructions (DO NOT RUN ANY OF THESE YET)

## Running Tests

```bash
mkdir -p build

gcc test/maintest.c \
    src/structures/structures.c \
    src/structures/structure_traversal/StructureTraversal.c \
    src/branch/branches.c \
    src/branch/merge/merge.c \
    src/commits/commits.c \
    src/storage/hash/hash.c \
    src/storage/compress/compress.c \
    src/storage/object/object.c \
    -Iinclude \
    -lz -lcrypto \
    -o build/test

./build/test
```

---

## Running Main Application

```bash
mkdir -p build

gcc -g main.c \
    src/structures/structures.c \
    src/structures/structure_traversal/StructureTraversal.c \
    src/branch/branches.c \
    src/commits/commits.c \
    src/storage/hash/hash.c \
    src/storage/compress/compress.c \
    src/storage/object/object.c \
    -Iinclude \
    -lz -lcrypto \
    -o build/main

./build/main

```

---

## Running Using the Cmake for the test

```bash

# Configure and create build folder
cmake -B build

# Compile the executable
cmake --build build

# Run the executable
./build/test

```

---

# Developer Roadmap

## Completed

- [x] Initial child node naming logic.
- [x] Initial commit creation routines.

---

## In Progress / Upcoming

- [ ] Reorganize source tree into `include/` and `src/storage/`
- [ ] Implement SHA hashing module
- [ ] Implement zlib compression wrapper
- [ ] Finish `write_object()`
- [ ] Redesign tree traversal logic
- [ ] Refactor merge system
- [ ] Enforce globally unique commit IDs
- [ ] Design commit stacking (branching from individual commits)
