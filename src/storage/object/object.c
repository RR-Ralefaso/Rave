#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "object.h"
#include "../compress/compress.h"

static int CreateDirectoryPath(const char *path)
{
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);
    size_t len = strlen(tmp);

    if (len == 0)
        return 0;

    for (unsigned i = 0; i < len;i++)
    {
        if (tmp[i] == '/' && i>0)
        {
            tmp[i] = '\0';
            mkdir(tmp, 0777);
            tmp[i] = '/';
        }
    }

    mkdir(tmp, 0777);
    return 0;
}


//writes an object to store

