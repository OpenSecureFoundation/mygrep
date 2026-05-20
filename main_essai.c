#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================================
   INCLUDES DES COLLABORATEURS
  
   ========================================================= */

/* ===== Collaborateur 1 ===== */


/* Fonctions du collaborateur 1 */
void invert_match(const char *pattern, FILE *f);
void highlight(const char *pattern, FILE *f);
void hide_filename(const char *pattern, FILE *f);
void label(const char *lbl,
           const char *pattern,
           FILE *f);

void null_data(const char *pattern, FILE *f);

void recursive_search(const char *dirname,
                      const char *pattern);

void no_ignore_case(const char *pattern,
                    FILE *f);

void extended_regex(const char *pattern,
                    FILE *f);


/* ===== Collaborateur 2 ===== */


/* Fonctions du collaborateur 2 */
void ignore_case_search(const char *pattern,
                        FILE *f);

void byte_offset_search(const char *pattern,
                        FILE *f);

void only_matching(const char *pattern,
                   FILE *f);

void exact_line_search(const char *pattern,
                       FILE *f);

void line_number(const char *pattern,
                 FILE *f);

void count_matches(const char *pattern,
                   FILE *f);


/* ===== Collaborateur 3 ===== */


/* Fonctions du collaborateur 3 */
void rechercher_fichier(const char *fichier);

void rechercher_recursif(const char *dossier);


/* =========================================================
   VARIABLES FLAGS
   ========================================================= */

int flag_v = 0;
int flag_h = 0;
int flag_f = 0;
int flag_L = 0;
int flag_z = 0;
int flag_r = 0;
int flag_c = 0;
int flag_e = 0;

int flag_i = 0;
int flag_b = 0;
int flag_o = 0;
int flag_x = 0;
int flag_n = 0;
int flag_count = 0;

/* =========================================================
   MAIN PRINCIPAL
   ========================================================= */

int main(int argc, char *argv[])
{
    int i;

    /* =====================================================
       ANALYSE DES OPTIONS
       ===================================================== */

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "-v") == 0)
                flag_v = 1;

            else if(strcmp(argv[i], "-h") == 0)
                flag_h = 1;

            else if(strcmp(argv[i], "-f") == 0)
                flag_f = 1;

            else if(strcmp(argv[i], "-l") == 0)
                flag_L = 1;

            else if(strcmp(argv[i], "-z") == 0)
                flag_z = 1;

            else if(strcmp(argv[i], "-r") == 0)
                flag_r = 1;

            else if(strcmp(argv[i], "-c") == 0)
                flag_c = 1;

            else if(strcmp(argv[i], "-e") == 0)
                flag_e = 1;

            else if(strcmp(argv[i], "-i") == 0)
                flag_i = 1;

            else if(strcmp(argv[i], "-b") == 0)
                flag_b = 1;

            else if(strcmp(argv[i], "-o") == 0)
                flag_o = 1;

            else if(strcmp(argv[i], "-x") == 0)
                flag_x = 1;

            else if(strcmp(argv[i], "-n") == 0)
                flag_n = 1;

            else if(strcmp(argv[i], "--count") == 0)
                flag_count = 1;
        }

        else
        {
            break;
        }
    }

    /* =====================================================
       VERIFICATION ARGUMENTS
       ===================================================== */

    if(argc - i < 2)
    {
        printf("Usage:\n");

        printf("./mygrep [OPTIONS] pattern file\n\n");

        printf("Options disponibles :\n");

        printf("  -v  invert match\n");
        printf("  -h  highlight\n");
        printf("  -f  hide filename\n");
        printf("  -l  list filename\n");
        printf("  -z  null data\n");
        printf("  -r  recursive search\n");
        printf("  -c  case sensitive\n");
        printf("  -e  regex\n");

        printf("  -i  ignore case\n");
        printf("  -b  byte offset\n");
        printf("  -o  only matching\n");
        printf("  -x  exact line\n");
        printf("  -n  line number\n");
        printf("  --count count matches\n");

        return EXIT_FAILURE;
    }

    /* =====================================================
       RECUPERATION ARGUMENTS
       ===================================================== */

    char *pattern = argv[i];
    char *target  = argv[i + 1];

    /* =====================================================
       RECHERCHE RECURSIVE
       ===================================================== */

    if(flag_r)
    {
        recursive_search(target, pattern);
        return EXIT_SUCCESS;
    }

    /* =====================================================
       OUVERTURE FICHIER
       ===================================================== */

    FILE *f;

    if(flag_z)
        f = fopen(target, "rb");

    else
        f = fopen(target, "r");

    if(f == NULL)
    {
        perror("Erreur ouverture fichier");
        return EXIT_FAILURE;
    }

    /* =====================================================
       APPELS DES FONCTIONS
       ===================================================== */

    if(flag_v)
    {
        invert_match(pattern, f);
    }

    else if(flag_h)
    {
        highlight(pattern, f);
    }

    else if(flag_f)
    {
        hide_filename(pattern, f);
    }

    else if(flag_L)
    {
        label("MATCH",
              pattern,
              f);
    }

    else if(flag_z)
    {
        null_data(pattern, f);
    }

    else if(flag_c)
    {
        no_ignore_case(pattern, f);
    }

    else if(flag_e)
    {
        extended_regex(pattern, f);
    }

    else if(flag_i)
    {
        ignore_case_search(pattern, f);
    }

    else if(flag_b)
    {
        byte_offset_search(pattern, f);
    }

    else if(flag_o)
    {
        only_matching(pattern, f);
    }

    else if(flag_x)
    {
        exact_line_search(pattern, f);
    }

    else if(flag_n)
    {
        line_number(pattern, f);
    }

    else if(flag_count)
    {
        count_matches(pattern, f);
    }

    /* =====================================================
       RECHERCHE NORMALE
       ===================================================== */

    else
    {
        char line[1024];

        while(fgets(line, sizeof(line), f))
        {
            if(strstr(line, pattern))
            {
                printf("%s", line);
            }
        }
    }

    fclose(f);

    return EXIT_SUCCESS;
}