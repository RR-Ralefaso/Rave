#include "object.h"
#include "../compress/compress.h"
#include "../hash/hash.h"
#include "../storing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <zlib.h>

// Writes objects into the directory .rave/objects using compress.c and hash.c
void WriteObject(const unsigned char *data, size_t len, const char *type) {
  // Error checking first
  if (data == NULL || type == NULL) {
    fprintf(stderr, "Error: data and/or type are NULL\n");
    return;
  }

  // Prepare the header: "type size\0" (e.g., "blob 1024\0")
  char header[64];
  int header_len = snprintf(header, sizeof(header), "%s %zu", type, len) +
                   1; // include null terminator

  size_t total_size = header_len + len;
  unsigned char *store_buffer = malloc(total_size);
  if (!store_buffer) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    return;
  }

  // Combine header and payload
  memcpy(store_buffer, header, header_len);
  memcpy(store_buffer + header_len, data, len);

  // ---> USES HASH.C: Compute SHA-256 hex string from memory buffer <---
  char *hash_hex = ComputeBuffer_sha256(store_buffer, total_size);
  if (!hash_hex) {
    fprintf(stderr, "Error: SHA-256 buffer hashing failed\n");
    free(store_buffer);
    return;
  }

  // Create base object directory (.rave/objects)
  int created = CreateDirectory(OBJECT_DIR);
  if (created == -1) {
    fprintf(stderr, "Failed to create Directory: %s\n", OBJECT_DIR);
    free(store_buffer);
    free(hash_hex);
    return;
  }

  // Create subdirectory using the first 2 characters of the hash
  char subDir[512];
  char subDirName[3] = {hash_hex[0], hash_hex[1], '\0'};

  snprintf(subDir, sizeof(subDir), "%s/%s", OBJECT_DIR, subDirName);
  if (CreateDirectory(subDir) == -1) {
    fprintf(stderr, "Failed to create sub-directory: %s\n", subDir);
    free(store_buffer);
    free(hash_hex);
    return;
  }

  // Open destination file for the compressed object
  char filePath[1024];
  snprintf(filePath, sizeof(filePath), "%s/%s", subDir, hash_hex + 2);

  FILE *dest = fopen(filePath, "wb");
  if (!dest) {
    fprintf(stderr, "Error: Failed to open file for writing: %s\n", filePath);
    free(store_buffer);
    free(hash_hex);
    return;
  }

  // Create a read-only memory stream from our store_buffer
  FILE *source = fmemopen(store_buffer, total_size, "rb");
  if (!source) {
    fprintf(stderr, "Error: Failed to create memory stream for compression\n");
    fclose(dest);
    free(store_buffer);
    free(hash_hex);
    return;
  }

  // ---> USES COMPRESS.C: Compress using compress_file_raw <---
  int ret = compress_file_raw(source, dest);

  // Cleanup streams and buffers
  fclose(source);
  fclose(dest);
  free(store_buffer);

  if (ret != Z_OK) {
    fprintf(
        stderr,
        "Error: Failed to compress object data using raw deflate (code %d)\n",
        ret);
    free(hash_hex);
    return;
  }

  printf("Object successfully written: %s\n", hash_hex);
  free(hash_hex);
}

// Reading the object using stream-based inflation counterpart
void ReadObject(const char *hash) {
  if (hash == NULL || strlen(hash) < 64) {
    fprintf(stderr, "Error: Invalid hash provided (expected SHA-256 hex)\n");
    return;
  }

  // Construct file path from hash (e.g., .rave/objects/ab/cdef...)
  char subDirName[3] = {hash[0], hash[1], '\0'};
  char filePath[1024];
  snprintf(filePath, sizeof(filePath), "%s/%s/%s", OBJECT_DIR, subDirName,
           hash + 2);

  FILE *fp = fopen(filePath, "rb");
  if (!fp) {
    fprintf(stderr, "Error: Object not found: %s\n", hash);
    return;
  }

  // Get file size to allocate buffer for reading compressed data
  fseek(fp, 0, SEEK_END);
  long compressed_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  unsigned char *compressed_data = malloc(compressed_size);
  if (!compressed_data) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    fclose(fp);
    return;
  }

  fread(compressed_data, 1, compressed_size, fp);
  fclose(fp);

  // Setup zlib stream for raw inflate (Window bits: -15)
  z_stream strm = {0};
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = compressed_size;
  strm.next_in = compressed_data;

  if (inflateInit2(&strm, -15) != Z_OK) {
    fprintf(stderr, "Error: Failed to initialize inflate\n");
    free(compressed_data);
    return;
  }

  // Dynamically grow uncompressed buffer as needed
  size_t uncompressed_capacity =
      compressed_size * 4 > 1024 ? compressed_size * 4 : 1024;
  unsigned char *uncompressed_data = malloc(uncompressed_capacity);
  size_t total_uncompressed_len = 0;
  int ret;

  do {
    strm.avail_out = CHUNK_SIZE;
    strm.next_out = uncompressed_data + total_uncompressed_len;

    // Ensure capacity
    if (total_uncompressed_len + CHUNK_SIZE > uncompressed_capacity) {
      uncompressed_capacity *= 2;
      unsigned char *temp = realloc(uncompressed_data, uncompressed_capacity);
      if (!temp) {
        fprintf(stderr, "Error: Memory reallocation failed during inflate\n");
        inflateEnd(&strm);
        free(compressed_data);
        free(uncompressed_data);
        return;
      }
      uncompressed_data = temp;
      strm.next_out = uncompressed_data + total_uncompressed_len;
    }

    ret = inflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
      fprintf(stderr, "Error: Inflation failed (code %d)\n", ret);
      inflateEnd(&strm);
      free(compressed_data);
      free(uncompressed_data);
      return;
    }

    total_uncompressed_len += CHUNK_SIZE - strm.avail_out;
  } while (ret != Z_STREAM_END);

  inflateEnd(&strm);

  printf("Successfully read object: %s (Size: %zu bytes)\n", hash,
         total_uncompressed_len);

  // Cleanup
  free(compressed_data);
  free(uncompressed_data);
}