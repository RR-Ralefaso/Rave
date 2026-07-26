#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Platform-specific setup for directory creation
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

/**
 * @brief Recursively creates a directory path across platforms (Windows and POSIX).
 *
 * @param path The null-terminated string containing the directory path to create.
 * @return int Returns 0 on success (or if directories already exist), or -1 on failure.
 */
static int CreateDirectory(const char *path)
{
    // Temporary buffer to hold the mutable path string during traversal
    char tmp[256];
    // Safely copy the input path into the temporary buffer
    snprintf(tmp, sizeof(tmp), "%s", path);
    size_t len = strlen(tmp);
    if (len == 0)
        return -1;
    // Remove any trailing slash/backslash to prevent empty directory components
    if (tmp[len - 1] == '/' || tmp[len - 1] == '\\')
        tmp[len - 1] = 0;
    // Iterate through the path string to find and create each directory level
    for (char *p = tmp + 1; *p; p++)
    {
        // Support both forward and backward slashes for cross-platform compatibility
        if (*p == '/' || *p == '\\')
        {
            // Temporarily terminate the string at the current path separator level
            *p = 0;
            // Attempt to create the intermediate directory; ignore EEXIST errors
            if (MKDIR(tmp) != 0 && errno != EEXIST)
                return -1;
            // Restore the path separator character for the next iteration
            *p = *(p - 1) == 0 ? '/' : *p;
        }
    }
    // Create the final directory component in the path
    if (MKDIR(tmp) != 0 && errno != EEXIST)
        return -1;
    return 0;
}