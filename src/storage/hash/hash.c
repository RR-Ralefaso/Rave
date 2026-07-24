#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include "hash.h"


/*
 * uses sha-256 hash of a file 
 * returns a dynamically allocated 65-character hex string (64 hex chars + '\0')
 * caller calls free() on the returned pointer
*/