#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define max_ligne 1024
#define MAX_BUFFER 100

// 🔹 fonction minuscule (-i)
void to_lowercase(char* str)
{
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// 🔹 vérifier extension (--include / --exclude)
int has_extension(const char* filename, const char* ext)
{
    return strstr(filename, ext) != NULL;
}

int main(int argc, char* argv[])
{
    FILE* f;
    char ligne[max_ligne];

    // 🔹 options existantes
    int ignore_case = 0;
    int after = 0, before = 0;

    char* include_ext = NULL;
    char* exclude_ext = NULL;

    // 🔹 nouvelle fonctionnalité -m
    int max_matches = -1;
    int match_count = 0;

    // 🔹 nouvelle fonctionnalité -b
    int show_byte_offset = 0;
    long byte_offset = 0;

    // 🔹 group separator
    char* group_separator = "--";
    int no_group_separator = 0;
    int first_match = 1;

    int arg_index = 1;

    // 🔹 analyse des options
    while (arg_index < argc && argv[arg_index][0] == '-') {

        // 🔹 -i
        if (strcmp(argv[arg_index], "-i") == 0) {
            ignore_case = 1;
        }

        // 🔹 -A
        else if (strcmp(argv[arg_index], "-A") == 0) {
            after = atoi(argv[++arg_index]);
        }

        // 🔹 -B
        else if (strcmp(argv[arg_index], "-B") == 0) {
            before = atoi(argv[++arg_index]);
        }

        // 🔹 -C
        else if (strcmp(argv[arg_index], "-C") == 0) {
            after = before = atoi(argv[++arg_index]);
        }

        // 🔹 --include
        else if (strcmp(argv[arg_index], "--include") == 0) {
            include_ext = argv[++arg_index];
        }

        // 🔹 --exclude
        else if (strcmp(argv[arg_index], "--exclude") == 0) {
            exclude_ext = argv[++arg_index];
        }

        // 🔹 -m
        else if (strcmp(argv[arg_index], "-m") == 0) {
            max_matches = atoi(argv[++arg_index]);
        }

        // 🔹 -b
        else if (strcmp(argv[arg_index], "-b") == 0) {
            show_byte_offset = 1;
        }

        // 🔹 --group-separator
        else if (strcmp(argv[arg_index], "--group-separator") == 0) {
            group_separator = argv[++arg_index];
        }

        // 🔹 --no-group-separator
        else if (strcmp(argv[arg_index], "--no-group-separator") == 0) {
            no_group_separator = 1;
        }

        arg_index++;
    }

    // 🔹 vérification arguments
    if (argc - arg_index < 2)
    {
        fprintf(stderr,
            "Usage: %s [OPTIONS] motif fichier...\n",
            argv[0]);

        return EXIT_FAILURE;
    }

    // 🔹 récupération motif
    char* motif = argv[arg_index++];

    // 🔹 copie motif pour -i
    char motif_lower[100];
    strcpy(motif_lower, motif);

    if (ignore_case)
        to_lowercase(motif_lower);

    // 🔹 buffer pour -B
    char buffer[MAX_BUFFER][max_ligne];
    int line_count = 0;

    // 🔹 parcours fichiers
    for (int i = arg_index; i < argc; i++) {

        char* filename = argv[i];

        // 🔹 include
        if (include_ext &&
            !has_extension(filename, include_ext))
            continue;

        // 🔹 exclude
        if (exclude_ext &&
            has_extension(filename, exclude_ext))
            continue;

        f = fopen(filename, "r");

        if (f == NULL) {
            perror("Erreur ouverture fichier");
            continue;
        }

        printf("\n=== %s ===\n", filename);

        // 🔹 lecture ligne par ligne
        while (fgets(ligne, sizeof(ligne), f) != NULL) {

            // 🔹 sauvegarde position byte
            long current_offset = byte_offset;

            // 🔹 mise à jour offset
            byte_offset += strlen(ligne);

            // 🔹 stocker ligne pour -B
            strcpy(buffer[line_count % MAX_BUFFER], ligne);
            line_count++;

            // 🔹 copie temporaire
            char temp[max_ligne];
            strcpy(temp, ligne);

            // 🔹 gestion -i
            if (ignore_case)
                to_lowercase(temp);

            // 🔹 recherche motif
            if (strstr(ignore_case ? temp : ligne,
                ignore_case ? motif_lower : motif)) {

                // 🔹 gestion -m
                if (max_matches != -1 &&
                    match_count >= max_matches)
                    break;

                // 🔹 séparateur groupes
                if (!first_match &&
                    !no_group_separator) {

                    printf("%s\n", group_separator);
                }

                first_match = 0;

                // 🔹 afficher lignes avant (-B)
                for (int j = line_count - before - 1;
                    j < line_count - 1;
                    j++) {

                    if (j >= 0)
                        printf("%s",
                            buffer[j % MAX_BUFFER]);
                }

                // 🔹 afficher offset (-b)
                if (show_byte_offset)
                    printf("%ld:", current_offset);

                // 🔹 ligne trouvée
                printf("%s", ligne);

                // 🔹 compteur matches
                match_count++;

                // 🔹 afficher lignes après (-A)
                for (int k = 0; k < after; k++) {

                    if (fgets(ligne,
                        sizeof(ligne),
                        f)) {

                        byte_offset += strlen(ligne);

                        printf("%s", ligne);
                    }
                }
            }
        }

        fclose(f);
    }

    return EXIT_SUCCESS;
}