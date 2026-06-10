
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max_ligne  1024
#define MAX_BUFFER 100

/* ============================================
   Fonction minuscule (-i)
   ============================================ */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

/* ============================================
   Vérifier extension (--include / --exclude)
   ============================================ */
int has_extension(const char *filename, const char *ext) {
    return strstr(filename, ext) != NULL;
}

/* ============================================
   Fonction principale de Teteya
   ============================================ */
void teteya_search(int argc, char *argv[]) {

    FILE *f;
    char  ligne[max_ligne];

    /* Options */
    int ignore_case       = 0;
    int after             = 0;
    int before            = 0;
    char *include_ext     = NULL;
    char *exclude_ext     = NULL;
    int max_matches       = -1;
    int match_count       = 0;
    int show_byte_offset  = 0;
    long byte_offset      = 0;
    char *group_separator = "--";
    int no_group_separator = 0;
    int first_match       = 1;
    int arg_index         = 1;

    /* Lecture des options */
    while (arg_index < argc && argv[arg_index][0] == '-') {

        if (strcmp(argv[arg_index], "-i") == 0)
            ignore_case = 1;

        else if (strcmp(argv[arg_index], "-A") == 0)
            after = atoi(argv[++arg_index]);

        else if (strcmp(argv[arg_index], "-B") == 0)
            before = atoi(argv[++arg_index]);

        else if (strcmp(argv[arg_index], "-C") == 0)
            after = before = atoi(argv[++arg_index]);

        else if (strcmp(argv[arg_index], "--include") == 0)
            include_ext = argv[++arg_index];

        else if (strcmp(argv[arg_index], "--exclude") == 0)
            exclude_ext = argv[++arg_index];

        else if (strcmp(argv[arg_index], "-m") == 0)
            max_matches = atoi(argv[++arg_index]);

        else if (strcmp(argv[arg_index], "-b") == 0)
            show_byte_offset = 1;

        else if (strcmp(argv[arg_index], "--group-separator") == 0)
            group_separator = argv[++arg_index];

        else if (strcmp(argv[arg_index], "--no-group-separator") == 0)
            no_group_separator = 1;

        arg_index++;
    }

    /* Vérification arguments */
    if (argc - arg_index < 2) {
        fprintf(stderr, "Usage: %s [OPTIONS] motif fichier...\n", argv[0]);
        return;
    }

    /* Récupération motif */
    char *motif = argv[arg_index++];

    /* Copie motif pour -i */
    char motif_lower[100];
    strcpy(motif_lower, motif);
    if (ignore_case)
        to_lowercase(motif_lower);

    /* Buffer pour -B */
    char buffer[MAX_BUFFER][max_ligne];
    int line_count = 0;

    /* Parcours des fichiers */
    for (int i = arg_index; i < argc; i++) {

        char *filename = argv[i];

        /* --include */
        if (include_ext && !has_extension(filename, include_ext))
            continue;

        /* --exclude */
        if (exclude_ext && has_extension(filename, exclude_ext))
            continue;

        f = fopen(filename, "r");
        if (f == NULL) {
            perror("Erreur ouverture fichier");
            continue;
        }

        printf("\n=== %s ===\n", filename);

        /* Lecture ligne par ligne */
        while (fgets(ligne, sizeof(ligne), f) != NULL) {

            long current_offset = byte_offset;
            byte_offset += strlen(ligne);

            strcpy(buffer[line_count % MAX_BUFFER], ligne);
            line_count++;

            char temp[max_ligne];
            strcpy(temp, ligne);

            if (ignore_case)
                to_lowercase(temp);

            /* Recherche motif */
            if (strstr(ignore_case ? temp : ligne,
                       ignore_case ? motif_lower : motif)) {

                /* -m : limite de résultats */
                if (max_matches != -1 && match_count >= max_matches)
                    break;

                /* Séparateur de groupes */
                if (!first_match && !no_group_separator)
                    printf("%s\n", group_separator);

                first_match = 0;

                /* -B : lignes avant */
                for (int j = line_count - before - 1;
                     j < line_count - 1; j++) {
                    if (j >= 0)
                        printf("%s", buffer[j % MAX_BUFFER]);
                }

                /* -b : offset en octets */
                if (show_byte_offset)
                    printf("%ld:", current_offset);

                /* Ligne trouvée */
                printf("%s", ligne);
                match_count++;

                /* -A : lignes après */
                for (int k = 0; k < after; k++) {
                    if (fgets(ligne, sizeof(ligne), f)) {
                        byte_offset += strlen(ligne);
                        printf("%s", ligne);
                    }
                }
            }
        }
        fclose(f);
    }
}

