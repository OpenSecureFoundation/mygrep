#include <stdio.h>
#include <string.h>

void highlight(char *line, char *pattern)
{
    int len = strlen(pattern);

    while (*line)
    {
        if (strncmp(line, pattern, len) == 0)
        {
            printf("[%s]", pattern);
            line += len;
        }
        else
        {
            putchar(*line);
            line++;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s pattern file\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[2], "r");

    if (!f)
    {
        perror("File error");
        return 1;
    }

    char line[1024];
    char *pattern = argv[1];

    while (fgets(line, sizeof(line), f))
    {
        highlight(line, pattern);
    }

    fclose(f);
    return 0;
}
