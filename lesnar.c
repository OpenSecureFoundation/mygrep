
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
int flag_I = 0;void lesnar_search(int argc, char *argv[]) {

    FILE *file;
    char  line[max_line];

    /* Lecture des flags */
    int i;
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

    /* Vérification fichier binaire (-I) */
    int I;
    while ((I = fgetc(file)) != EOF) {
        if (I == '\0') {
            if (flag_I == 1) {
                fclose(file);
                return;
            } else {
                printf("match found\n");
                fclose(file);
                return;
            }
        }
    }
    rewind(file);
    /* Remet le curseur au début après vérification */

    /* Gestion flag -d */
    int d = 0;
    if (flag_d == 1) {
        for (i = 1; i < argc - 2; i++) {
            if (strcmp(argv[i], "-d") == 0)
                d = i + 1;
            else
                i = i + 1;
        }
        if ((strcmp(argv[d + 1], "read") == 0) ||
            (strcmp(argv[d + 1], "skip") == 0)) {
            printf("error\n");
            fclose(file);
            return;
        }
        if (strcmp(argv[d + 1], "recurse") == 0) {
            /* appel recursif à gérer dans le main */
        } else {
            printf("invalid action\n");
        }
    }

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
                    if (flag_Z == 1) {
                        char nul = '\0';
                        printf("%s%c", argv[argc - 1], nul);
                    } else {
                        printf("%s\n", argv[argc - 1]);
                    }
                    fclose(file);
                    return;
                }
                if (flag_n == 1)
                    printf("%d: %s\n", n, line);
                if (flag_q == 1) {
                    fclose(file);
                    return;
                }
                if (flag_F == 1) {
                    printf("%s\n", line);
                    fclose(file);
                    return;
                } else {
                    printf("%s\n", line);
                }
            }
        }
    }

    fclose(file);
}

