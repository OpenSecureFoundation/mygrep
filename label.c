#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s label pattern\n", argv[0]);
        return 1;
    }

    char *label = argv[1];
    char *pattern = argv[2];

    char line[MAX_LINE];

    // Read from standard input
    while (fgets(line, sizeof(line), stdin))
    {
        if (strstr(line, pattern) != NULL)
        {
            printf("%s:%s", label, line);
        }
    }

    return 0;
}
