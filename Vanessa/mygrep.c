#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <regex.h>

#define MAX_LINE 1024
#define MAX_PATH 1024

/* ─────────────────────────────────────────
   1. INVERT MATCH
   Prints lines that do NOT contain the pattern
   ───────────────────────────────────────── */
void invert_match(const char *pattern, FILE *f)
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern) == NULL)
        {
            printf("%s", line);
        }
    }
}

/* ─────────────────────────────────────────
   2. HIGHLIGHT MATCHES
   Wraps matched pattern in brackets [ ]
   ───────────────────────────────────────── */
void highlight_line(char *line, const char *pattern)
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

void highlight(const char *pattern, FILE *f)
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern))
            highlight_line(line, pattern);
    }
}

/* ─────────────────────────────────────────
   3. HIDE FILENAME
   Prints matching lines without showing filename
   ───────────────────────────────────────── */
void hide_filename(const char *pattern, FILE *f)
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern))
        {
            printf("%s", line);
        }
    }
}

/* ─────────────────────────────────────────
   4. LABEL
   Prints matching lines prefixed with a label
   ───────────────────────────────────────── */
void label(const char *lbl, const char *pattern, FILE *f)
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern) != NULL)
        {
            printf("%s:%s", lbl, line);
        }
    }
}

/* ─────────────────────────────────────────
   5. NULL DATA
   Searches null-separated records in binary files
   ───────────────────────────────────────── */
void null_data(const char *pattern, FILE *f)
{
    char buffer[MAX_LINE];
    int index = 0;
    int ch;

    while ((ch = fgetc(f)) != EOF)
    {
        if (ch == '\0')
        {
            buffer[index] = '\0';
            if (strstr(buffer, pattern) != NULL)
            {
                printf("%s\n", buffer);
            }
            index = 0;
        }
        else
        {
            if (index < MAX_LINE - 1)
                buffer[index++] = ch;
        }
    }
}

/* ─────────────────────────────────────────
   6. RECURSIVE SEARCH
   Searches all files in a directory recursively
   ───────────────────────────────────────── */
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
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        if (S_ISDIR(path_stat.st_mode))
            recursive_search(path, pattern);
        else
            search_file(path, pattern);
    }
    closedir(dir);
}

/* ─────────────────────────────────────────
   7. NO IGNORE CASE (case-sensitive search)
   Default grep behavior, explicit case-sensitive
   ───────────────────────────────────────── */
void no_ignore_case(const char *pattern, FILE *f)
{
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, pattern) != NULL)
        {
            printf("%s", line);
        }
    }
}

/* ─────────────────────────────────────────
   8. EXTENDED REGEX
   Searches using extended regular expressions
   ───────────────────────────────────────── */
void extended_regex(const char *pattern, FILE *f)
{
    regex_t regex;

    if (regcomp(&regex, pattern, REG_EXTENDED))
    {
        printf("Invalid regular expression\n");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f))
    {
        if (regexec(&regex, line, 0, NULL, 0) == 0)
        {
            printf("%s", line);
        }
    }
    regfree(&regex);
}

/* ─────────────────────────────────────────
   MAIN — calls each function based on flag
   Usage: ./mygrep_vanessa -<flag> pattern file/dir
   Flags:
     -v  invert match
     -h  highlight
     -f  hide filename
     -l  label (needs label as extra arg)
     -z  null data
     -r  recursive
     -c  no ignore case
     -e  extended regex
   ───────────────────────────────────────── */
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s -<flag> pattern file/dir [label]\n", argv[0]);
        printf("Flags:\n");
        printf("  -v  Invert match\n");
        printf("  -h  Highlight matches\n");
        printf("  -f  Hide filename\n");
        printf("  -l  Label (provide label as 4th argument)\n");
        printf("  -z  Null data\n");
        printf("  -r  Recursive search\n");
        printf("  -c  No ignore case (case-sensitive)\n");
        printf("  -e  Extended regex\n");
        return 1;
    }

    char *flag    = argv[1];
    char *pattern = argv[2];

    /* ── Recursive search uses directory, not file ── */
    if (strcmp(flag, "-r") == 0)
    {
        if (argc != 4)
        {
            printf("Usage: %s -r pattern directory\n", argv[0]);
            return 1;
        }
        recursive_search(argv[3], pattern);
        return 0;
    }

    /* ── Label needs an extra argument ── */
    if (strcmp(flag, "-l") == 0)
    {
        if (argc != 5)
        {
            printf("Usage: %s -l pattern file label\n", argv[0]);
            return 1;
        }
        FILE *f = fopen(argv[3], "r");
        if (!f) { perror("File error"); return 1; }
        label(argv[4], pattern, f);
        fclose(f);
        return 0;
    }

    /* ── All other functions need a file ── */
    if (argc != 4)
    {
        printf("Usage: %s %s pattern file\n", argv[0], flag);
        return 1;
    }

    /* ── Null data opens in binary mode ── */
    if (strcmp(flag, "-z") == 0)
    {
        FILE *f = fopen(argv[3], "rb");
        if (!f) { perror("File error"); return 1; }
        null_data(pattern, f);
        fclose(f);
        return 0;
    }

    /* ── All other functions open in text mode ── */
    FILE *f = fopen(argv[3], "r");
    if (!f) { perror("File error"); return 1; }

    if      (strcmp(flag, "-v") == 0) invert_match(pattern, f);
    else if (strcmp(flag, "-h") == 0) highlight(pattern, f);
    else if (strcmp(flag, "-f") == 0) hide_filename(pattern, f);
    else if (strcmp(flag, "-c") == 0) no_ignore_case(pattern, f);
    else if (strcmp(flag, "-e") == 0) extended_regex(pattern, f);
    else
    {
        printf("Unknown flag: %s\n", flag);
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}
