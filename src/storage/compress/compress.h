#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>



//================chunk sizes============
#define CHUNK_SIZE 16384

//prototype functions
int CompressFileRaw(FILE *source, FILE *destination);

#endif