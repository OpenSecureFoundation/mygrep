#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_ligne 1024

/* ============================================
   Recherche simple sans options
   ============================================ */
void ninkam_search(const char *motif, const char *filename) {

    FILE *f;
    char  ligne[max_ligne];

    f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, motif) != NULL)
            printf("%s", ligne);
    }

    fclose(f);
}