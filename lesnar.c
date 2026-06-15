#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_line 1024

/* Flags de Lesnar */
int flag_n = 0;
int flag_l = 0;
int flag_s = 0;
int flag_F = 0;
int flag_q = 0;
int flag_Z = 0;
int flag_d = 0;
int flag_I = 0;

/* ============================================
   Fonction -Z : afficher avec caractere nul
   ============================================ */
void flag_Z_function(char *filename) {
    char nul = '\0';
    printf("%s%c", filename, nul);
}

/* ============================================
   Fonction -l : afficher nom du fichier
   ============================================ */
int flag_l_function(int flag_Z, char *filename, FILE *file) {
    if (flag_Z == 1)
        flag_Z_function(filename);
    else
        printf("%s\n", filename);
    fclose(file);
    return EXIT_SUCCESS;
}

/* ============================================
   Fonction -n : afficher numero de ligne
   ============================================ */
void flag_n_function(int n, char *line) {
    printf("%d: %s\n", n, line);
}

/* ============================================
   Fonction -q : mode silencieux
   ============================================ */
int flag_q_function(FILE *file) {
    fclose(file);
    return EXIT_SUCCESS;
}

/* ============================================
   Fonction -F : chaine fixe
   ============================================ */
int flag_F_function(char *line, FILE *file) {
    printf("%s\n", line);
    fclose(file);
    return EXIT_SUCCESS;
}

/* ============================================
   Fonction -I : ignorer fichiers binaires
   ============================================ */
int flag_I_function(FILE *file) {
    fclose(file);
    return EXIT_FAILURE;
}

/* ============================================
   Fonction -d : gestion des dossiers
   ============================================ */
int flag_d_function(int argc, char *argv[]) {
    int d = 0;
    int i;

    for (i = 1; i < argc - 2; i++) {
        if (strcmp(argv[i], "-d") == 0)
            d = i + 1;
        else
            i = i + 1;
    }

    if ((strcmp(argv[d + 1], "read") == 0) ||
        (strcmp(argv[d + 1], "skip") == 0)) {
        printf("error");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[d + 1], "recurse") == 0) {
        /* appel recursif a gerer dans le main */
    } else {
        printf("invalid action");
    }

    return EXIT_SUCCESS;
}

/* ============================================
   Fonction principale de Lesnar
   ============================================ */
void lesnar_search(int argc, char *argv[]) {

    FILE *file;
    char  line[max_line];
    int   i;

    /* Lecture des flags */
    for (i = 1; i < argc - 2; i++) {
        if (argv[i][0] == '-') {
            if      (strcmp(argv[i], "-n") == 0) flag_n = 1;
            else if (strcmp(argv[i], "-l") == 0) flag_l = 1;
            else if (strcmp(argv[i], "-s") == 0) flag_s = 1;
            else if (strcmp(argv[i], "-F") == 0) flag_F = 1;
            else if (strcmp(argv[i], "-q") == 0) flag_q = 1;
            else if (strcmp(argv[i], "-Z") == 0) flag_Z = 1;
            else if (strcmp(argv[i], "-d") == 0) flag_d = 1;
            else if (strcmp(argv[i], "-I") == 0) flag_I = 1;
            else printf("flag not recognized\n");
        }
    }

    /* Ouverture du fichier */
    file = fopen(argv[argc - 1], "r");
    if (file == NULL) {
        if (flag_s == 1)
            return;
        else {
            perror("error in opening file");
            return;
        }
    }

    /* Verification fichier binaire */
    int I;
    while ((I = fgetc(file)) != EOF) {
        if (I == '\0') {
            if (flag_I == 1) {
                flag_I_function(file);
                return;
            } else {
                printf("match found\n");
                fclose(file);
                return;
            }
        }
    }

    /* Gestion flag -d */
    if (flag_d == 1)
        flag_d_function(argc, argv);

    rewind(file);

    /* Recherche ligne par ligne */
    int n = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        n = n + 1;
        if (strstr(line, argv[argc - 2]) != NULL) {
            if (argc == 3) {
                printf("%s", line);
            }
            if (argc > 3) {
                if (flag_l == 1) {
                    flag_l_function(flag_Z, argv[argc - 1], file);
                    return;
                }
                if (flag_n == 1)
                    flag_n_function(n, line);
                if (flag_q == 1) {
                    flag_q_function(file);
                    return;
                }
                if (flag_F == 1) {
                    flag_F_function(line, file);
                    return;
                } else {
                    printf("%s\n", line);
                }
            }
        }
    }

    fclose(file);
}