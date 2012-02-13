/*
    Illustration of basic directory reading functions. Given a list of
    files/directories recursively list its contents, with owner and access
    rights of this process.
*/
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

#define PATH_LENGTH 1024

void printPermission(mode_t mode);
void processDirectory(const char* path);
void printType(mode_t mode);

void examine(const char* path)
{
    struct stat info;
    printf("%s: ", path);
    stat(path, &info);
    printType(info.st_mode);
    printPermission(info.st_mode);
    if (S_ISDIR(info.st_mode))
    {
        putchar('\n');
        processDirectory(path);
    }
    else
    {
        /* Get owner name from /etc/passwd */
        struct passwd* pw = getpwuid(info.st_uid);
        printf("owner: %s, ", pw->pw_name);
        putchar('\n');
    }   
    
}

void printPermission(mode_t mode)
{
    char buffer[10];
    buffer[9] = 0;
    int i = 0;
    buffer[i ++] = S_IRUSR & mode ? 'r' : '-';
    buffer[i ++] = S_IWUSR & mode ? 'w' : '-';
    buffer[i ++] = S_IXUSR & mode ? 'x' : '-';
    buffer[i ++] = S_IRGRP & mode ? 'r' : '-';
    buffer[i ++] = S_IWGRP & mode ? 'w' : '-';
    buffer[i ++] = S_IXGRP & mode ? 'x' : '-';
    buffer[i ++] = S_IROTH & mode ? 'r' : '-';
    buffer[i ++] = S_IWOTH & mode ? 'w' : '-';
    buffer[i ++] = S_IXOTH & mode ? 'x' : '-';
    printf("%s ", buffer);
}

void processDirectory(const char* path)
{
    struct dirent* entry = 0;
    char buffer[PATH_LENGTH];
    strncpy(buffer, path, PATH_LENGTH);
    char* ptr = buffer + strlen(path);
    DIR* dir = opendir(path);
    
    while ((entry = readdir(dir)))
    {
        /* Omit current/parent directory - infinite loop */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        strcpy(ptr, entry->d_name);
        examine(buffer);
    }
    closedir(dir);
}

void printType(mode_t mode)
{
    /* Definitely not exhaustive, but whatever */
    char* type = 0;
    if (S_ISREG(mode))
    {
        type = "regular";
    }
    else if (S_ISDIR(mode))
    {
        type = "directory";
    }
    else
    {
        type = "sth else";
    }
    printf("%s ", type);
}

int main(int argc, char* argv[])
{
    int i;
    for (i = 1; i < argc; ++ i)
    {
        examine(argv[i]);
    }
    return 0;
}
