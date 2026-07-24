#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//scans the directory recursively
void ScanProjectDir(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if(!dir)
        return;

    struct dirent *entry;

    while((entry = readdir(dir)!=NULL))
    {
        //ignoring "." and ".." to prevent infinite loops
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..")==0)continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat path_stat; 
        if(stat(full_path,&path_stat) ==0 )
        {
            if(S_ISDIR(path_stat.st_mode))
            {
                //recurse into sub dir
                ScanProjectDir(full_path);
            }
            else if (S_ISDIR(path_stat.st_mode))
            {
                //checks if  file ends with ".xcf , .psd , .kra , or .krz"
                if(strstr(entry->d_name,".xcf") || strstr(entry->d_name,".psd") || strstr(entry->d_name,".kra") || strstr(entry->d_name,".krz"))
                {
                    printf("Found project file : %s\n", full_path);
                    //TODO : CHECK IF HASH HAS CHANGED OR NAH
                }
            }
        }
    }
    closedir(dir);
}



