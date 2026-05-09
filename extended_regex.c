#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

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

    regex_t regex;

    // Compile extended regular expression
    if (regcomp(&regex, pattern, REG_EXTENDED))
    {
        printf("Invalid regular expression\n");
        fclose(f);
        return 1;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), f))
    {
        // Check if line matches regex
        if (regexec(&regex, line, 0, NULL, 0) == 0)
        {
            printf("%s", line);
        }
    }

    regfree(&regex);
    fclose(f);

    return 0;
}
