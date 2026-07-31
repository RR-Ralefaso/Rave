// its header file
#include "object.h"
// will be used to store
#include "../storing.h"
#include <stdio.h>
#include <stdlib.h>
// using strings
#include <string.h>
#include <sys/types.h>
// does a lot of direcotry checks
#include "../../checker/checks.h"

// Writing the objects

// writes objects into the directory .rave/objects
void WriteObject(const unsigned char *data, size_t len, const char *type) {
  // error checkig first
  if (data == NULL || type == NULL) {
    fprintf(stderr, "Error : data and or type are NUll\n");
    return;
  }

  // locking to see which directory we in
  // debugging purposes should comment out later
  char *whichdirectory_path = GetCurrentDirectory();
  if (whichdirectory_path == NULL) {
    fprintf(stderr, "Error\n");
    return;
  } else {
    printf("Current path : %s\n", whichdirectory_path);
  }
}

// reading the object
void ReadObject(const char *hash) {
  // TODO : create a way of reading objects
}
