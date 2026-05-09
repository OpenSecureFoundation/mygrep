#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s pattern file\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    FILE *f = fopen(argv[2], "r");

    if (!f)
    {
        perror("File error");
        return 1;
    }

    char line[1024];

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern))
        {
            printf("%s", line);
        }
    }

    fclose(f);
    return 0;
}
