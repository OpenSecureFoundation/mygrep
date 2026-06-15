#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_ligne 1024

/* ============================================
   Recherche depuis stdin (entrée standard)
   ============================================ */
void ninkam_stdin(const char *motif) {

    FILE *f;
    char  ligne[max_ligne];

    f = stdin;

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, motif) != NULL)
            printf("%s", ligne);
    }
}