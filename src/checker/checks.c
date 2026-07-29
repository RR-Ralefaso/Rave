#include "checks.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <errno.h>
#include <fcntl.h>
#include <libproc.h>
#include <sys/file.h>
#include <sys/proc_info.h>
#include <unistd.h>
#elif defined(__linux__)
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#endif

// checks if a file ends with a specific extension
int HasExtension(const char *filename, const char *ext) {
  size_t len = strlen(filename);
  size_t ext_len = strlen(ext);
  if (len < ext_len)
    return 0;
  return strcmp(filename + len - ext_len, ext) == 0;
}

#ifdef _WIN32

// checks if file is open on windows
int IsFileOpen_WIN(const char *filepath) {
  HANDLE hfile = CreateFileA(filepath, GENERIC_READ | GENERIC_WRITE,
                             0, // 0 share mode = exclusive access request
                             NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hfile == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError();
    // file is locked or open somewhere else
    if (error == ERROR_SHARING_VIOLATION || error == ERROR_LOCK_VIOLATION) {
      return 1;
    }
    return -1; // file missing or no permission or whatever
  }

  CloseHandle(hfile);
  return 0; // file is free
}

#elif defined(__APPLE__)

// checks if file is open on macos
int IsFileOpen_macOS(const char *filepath) {
  char abs_path[PATH_MAX];
  if (realpath(filepath, abs_path) == NULL) {
    return -1; // path bad or file gone
  }

  // get all active process pids
  int num_pids = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
  if (num_pids <= 0)
    return -1;

  pid_t pids[num_pids];
  num_pids = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

  pid_t my_pid = getpid();

  // loop through processes to see if anyone opened our file
  for (int i = 0; i < num_pids / (int)sizeof(pid_t); i++) {
    if (pids[i] <= 0 || pids[i] == my_pid)
      continue;

    int buffer_size = proc_pidinfo(pids[i], PROC_PIDLISTFDS, 0, NULL, 0);
    if (buffer_size <= 0)
      continue;

    struct proc_fdinfo fds[buffer_size / sizeof(struct proc_fdinfo)];
    buffer_size = proc_pidinfo(pids[i], PROC_PIDLISTFDS, 0, fds, sizeof(fds));

    int fd_count = buffer_size / sizeof(struct proc_fdinfo);
    for (int j = 0; j < fd_count; j++) {
      if (fds[j].proc_fdtype == PROX_FDTYPE_VNODE) {
        struct vnode_fdinfo vnode_info;
        if (proc_pidfdinfo(pids[i], fds[j].proc_fd, PROC_PIDFDVNODEPATHINFO,
                           &vnode_info, sizeof(vnode_info)) > 0) {
          if (strcmp(vnode_info.pvip.vip_path, abs_path) == 0) {
            return 1; // found a process holding this file open
          }
        }
      }
    }
  }

  return 0; // nobody has it open
}

#elif defined(__linux__)

// checks if file is open on linux
int IsFileOpen_Linux(const char *filepath) {
  int fd = open(filepath, O_RDWR);
  if (fd == -1) {
    if (errno == EACCES || errno == EPERM)
      return 1;
    return -1;
  }

  // try non-blocking lock
  if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      close(fd);
      return 1; // locked by another process
    }
  }

  flock(fd, LOCK_UN);
  close(fd);
  return 0; // file is free
}

#endif

// cross platform wrapper to check if file is open
int IsFileOpen(const char *filepath) {
#ifdef _WIN32
  return IsFileOpen_WIN(filepath);
#elif defined(__APPLE__)
  return IsFileOpen_macOS(filepath);
#elif defined(__linux__)
  return IsFileOpen_Linux(filepath);
#else
  return 0;
#endif
}

// scans the directory recursively
void ScanProjectDir(const char *dir_path) {
  DIR *dir = opendir(dir_path);
  if (!dir)
    return;

  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    // ignoring "." and ".." to prevent infinite loops
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

    struct stat path_stat;
    if (stat(full_path, &path_stat) == 0) {
      if (S_ISDIR(path_stat.st_mode)) {
        // recurse into sub dir
        ScanProjectDir(full_path);
      } else if (S_ISREG(path_stat.st_mode)) {
        // checks if file ends with ".xcf" , ".psd" , ".kra" , or ".krz"
        if (HasExtension(entry->d_name, ".xcf") ||
            HasExtension(entry->d_name, ".psd") ||
            HasExtension(entry->d_name, ".kra") ||
            HasExtension(entry->d_name, ".krz")) {
          printf("Found project file : %s\n", full_path);

          int open_status = IsFileOpen(full_path);
          if (open_status == 1) {
            printf("  -> File is open in another program!\n");
          } else if (open_status == 0) {
            printf("  -> File is closed.\n");
            // TODO : CHECK IF HASH HAS CHANGED OR NAH
          } else {
            printf("  -> Could not check file status.\n");
          }
        }
      }
    }
  }
  closedir(dir);
}

/*
    TODO : write DirExists
*/

// return 0 if the directory exists and -1 if it doesnt
int DirExists(const char *dir_path) {}