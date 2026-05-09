#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s pattern file\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    char *filename = argv[2];

    FILE *f = fopen(filename, "r");

    if (f == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), f))
    {
        // Case-sensitive search (default behavior)
        if (strstr(line, pattern) != NULL)
        {
            printf("%s", line);
        }
    }

    fclose(f);

    return 0;
}
