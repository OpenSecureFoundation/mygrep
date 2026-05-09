#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LINE 1024
#define MAX_PATH 1024

void search_file(const char *filepath, const char *pattern)
{
    FILE *f = fopen(filepath, "r");

    if (f == NULL)
        return;

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern) != NULL)
        {
            printf("%s: %s", filepath, line);
        }
    }

    fclose(f);
}

void recursive_search(const char *dirname, const char *pattern)
{
    DIR *dir = opendir(dirname);

    if (dir == NULL)
        return;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        // Ignore . and ..
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char path[MAX_PATH];

        snprintf(path, sizeof(path), "%s/%s",
                 dirname, entry->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        // If directory -> recurse
        if (S_ISDIR(path_stat.st_mode))
        {
            recursive_search(path, pattern);
        }
        else
        {
            search_file(path, pattern);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s pattern directory\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    char *directory = argv[2];

    recursive_search(directory, pattern);

    return 0;
}
