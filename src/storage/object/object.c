#include "object.h"
#include "../storing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Writing the objects

// writes objects into the directory .rave/objects
void WriteObject(const unsigned char *data, size_t len, const char *type) {
  // error checkig first
  if (data == NULL || type == NULL) {
    fprintf(stderr, "Error : data and or type are NUll\n");
    return;
  }

  /*
      TODO : use a function called DirExists to see if the directory exists
  */
}

// reading the object
void ReadObject(const char *hash) {
  // TODO : create a way of reading objects
}
