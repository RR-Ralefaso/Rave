cmake_minimum_required(VERSION 3.14)
project(RAVE_TEST C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories (-Iinclude)
include_directories(include)

# Find required libraries (-lz -lcrypto)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

# Define source files
set(SOURCES
    test/maintest.c
    src/structures/structures.c
    src/structures/structure_traversal/StructureTraversal.c
    src/branch/branches.c
    src/branch/merge/merge.c
    src/commits/commits.c
    src/storage/hash.c
    src/storage/compress.c
    src/storage/object.c
)

# Define executable
add_executable(test ${SOURCES})

# Link libraries
target_link_libraries(test PRIVATE 
    ZLIB::ZLIB 
    OpenSSL::Crypto
)