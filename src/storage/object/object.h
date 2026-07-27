#include <stdio.h>
#include <stdlib.h>
#include "../compress/compress.h"


#define OBJECT_DIR ".rave/objects"


//prototype functions
void WriteObject(const unsigned char *data, size_t len, const char *type);
void ReadObject(const char *hash);