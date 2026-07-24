#ifndef CHECKS_H
#define CHECKS_H

#include <stdlib.h>
#include <stdbool.h>


//--Protype functions
int IsFileOpen(const char *filepath);
int HasExtension(const char *filename, const char *ext);
void ScanProjectDir(const char *dir_path);

#endif