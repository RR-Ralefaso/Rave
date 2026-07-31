#include "hash.h"
#include "../object/object.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * uses sha-256 hash of a file
 * returns a dynamically allocated 65-character hex string (64 hex chars + '\0')
 * caller calls free() on the returned pointer
 *
 *
 * ZAHAHAHA IM POLARIS(THIS IS USELESS)
 *
 *
 */

char *ComputeFile_sha256(const char *filepath) {
  FILE *file = fopen(filepath, "rb");

  if (!file) {
    // the file didnt open -> bad so error
    fprintf(stderr, "Error : failed to open the file\n");
    return NULL;
  }

  // allocate on openssl digest context
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx) {
    fclose(file);
    return NULL;
  }

  //  initialise sha256 context
  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    return NULL;
  }

  unsigned char buffer[BUFFER_SIZE];
  size_t bytes_read;

  // stream the file in chuncks to keep memory usage low

  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    if (EVP_DigestUpdate(mdctx, buffer, bytes_read) != 1) {
      EVP_MD_CTX_free(mdctx);
      fclose(file);
      return NULL;
    }
  }

  // final digest execution
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hash_len = 0;

  if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
    EVP_MD_CTX_free(mdctx);
    fclose(file);
    return NULL;
  }

  // cleaning after myself (get it hehe ok my bad)

  EVP_MD_CTX_free(mdctx);
  fclose(file); // best prcatise to alawys close  :)

  // change raw byte into hex string ending with "\0"
  // 32 bytes * 2 chars/byte +1 null terminator = 65 bytes or something
  char *hexstr = malloc(hash_len * 2 + 1);
  if (!hexstr)
    return NULL;

  for (unsigned int i = 0; i < hash_len; i++) {
    sprintf(hexstr + (i * 2), "%02x", hash[i]);
  }
  hexstr[hash_len * 2] = '\0';

  return hexstr;
}

// Helper function to convert binary hash to a 40-character hex string
void HashToString(const unsigned char *hash, char *output) {
  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    sprintf(output + (i * 2), "%02x", hash[i]);
  }
  output[SHA_DIGEST_LENGTH * 2] = '\0';
}

// Computes SHA-256 of an in-memory buffer and returns a 65-character hex string
char *ComputeBuffer_sha256(const unsigned char *data, size_t len) {
  if (!data)
    return NULL;

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  if (!mdctx)
    return NULL;

  if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1 ||
      EVP_DigestUpdate(mdctx, data, len) != 1) {
    EVP_MD_CTX_free(mdctx);
    return NULL;
  }

  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hash_len = 0;

  if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
    EVP_MD_CTX_free(mdctx);
    return NULL;
  }

  EVP_MD_CTX_free(mdctx);

  char *hexstr = malloc(hash_len * 2 + 1);
  if (!hexstr)
    return NULL;

  for (unsigned int i = 0; i < hash_len; i++) {
    sprintf(hexstr + (i * 2), "%02x", hash[i]);
  }
  hexstr[hash_len * 2] = '\0';

  return hexstr;
}

void StageFile(const char *filepath) {
  // Get the SHA-256 hash directly from the file on disk using hash.c
  char *file_hash = ComputeFile_sha256(filepath);
  if (!file_hash) {
    fprintf(stderr, "Failed to hash file: %s\n", filepath);
    return;
  }

  printf("File hashed successfully! Hash: %s\n", file_hash);

  // Read the file contents into memory to write it as an object
  FILE *f = fopen(filepath, "rb");
  if (!f) {
    free(file_hash);
    return;
  }

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  unsigned char *buffer = malloc(size);
  fread(buffer, 1, size, f);
  fclose(f);

  //Write it out as a compressed object
  WriteObject(buffer, size, "blob");

  free(buffer);
  free(file_hash);
}