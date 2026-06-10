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

