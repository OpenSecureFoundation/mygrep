#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s pattern file\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    char *filename = argv[2];

    FILE *f = fopen(filename, "rb");

    if (f == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_BUFFER];
    int index = 0;
    int ch;

    while ((ch = fgetc(f)) != EOF)
    {
        if (ch == '\0')
        {
            buffer[index] = '\0';

            // Check pattern inside null-separated data
            if (strstr(buffer, pattern) != NULL)
            {
                printf("%s\n", buffer);
            }

            index = 0;
        }
        else
        {
            buffer[index++] = ch;
        }
    }

    fclose(f);

    return 0;
}
