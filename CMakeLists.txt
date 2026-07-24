cmake_minimum_required(VERSION 3.28)
project(RAVE_TEST C)

# Set the standard to C23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON) # Optional: Enforces strict C23 standard compliance without compiler extensions

# Find required dependencies
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

# Collect source files
set(SOURCES
    test/maintest.c
    src/structures/structures.c
    src/structures/structure_traversal/StructureTraversal.c
    src/branch/branches.c
    src/branch/merge/merge.c
    src/commits/commits.c
    src/storage/hash/hash.c
    src/storage/compress/compress.c
    src/storage/object/object.c
)

# Define executable target
add_executable(test ${SOURCES})

# Modern target-scoped include directories
target_include_directories(test PRIVATE include)

# Link imported library targets
target_link_libraries(test PRIVATE 
    ZLIB::ZLIB 
    OpenSSL::Crypto
)