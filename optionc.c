#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_ligne 1024

/* ============================================
   -c : compter les lignes correspondantes
   ============================================ */
void ninkam_count(const char *motif, const char *filename) {

    FILE *f;
    char  ligne[max_ligne];
    int   nb_lignes = 0;

    f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, motif) != NULL)
            nb_lignes++;
    }

    printf("%d\n", nb_lignes);
    fclose(f);
}