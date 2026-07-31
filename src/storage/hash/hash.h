#ifndef HASH_H
#define HASH_H

// reads 8kb of data at a time
#define BUFFER_SIZE 8129
#include <stddef.h>

// prototpe function

// used to create a hashed version
char *ComputeFile_sha256(const char *filepath);

// Helper function to convert binary hash to a 40-character hex string
void HashToString(const unsigned char *hash, char *output);

// when adding a file
void StageFile(const char *filepath);

/* computes the SHA-256 hash of an in-memory data buffer and returns it as a hex
 string*/
char *ComputeBuffer_sha256(const unsigned char *data, size_t len);

#endif