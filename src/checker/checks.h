#ifndef CHECKS_H
#define CHECKS_H

#include <stdbool.h>
#include <stdlib.h>

//--Protype functions

// checks if file is open on windows
int IsFileOpen_WIN(const char *filepath);

// checks if file is open on macos
int IsFileOpen_macOS(const char *filepath);

// checks if file is open on linux
int IsFileOpen_Linux(const char *filepath);

// cross platform wrapper to check if file is open
int IsFileOpen(const char *filepath);
// checks if a file ends with a specific extension -- helper function
int HasExtension(const char *filename, const char *ext);

// scans the directory recursively
void ScanProjectDir(const char *dir_path);

#endif