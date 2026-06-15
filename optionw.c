#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max_ligne 1024

/* ============================================
   Vérifie si un caractère fait partie d'un mot
   ============================================ */
int est_char_mot(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

/* ============================================
   Vérifie si le motif est un mot entier
   ============================================ */
int est_mot_entier(char *ligne, char *motif) {
    char *pos;
    int   gauche, droite;
    int   longueur = strlen(motif);

    pos = strstr(ligne, motif);
    if (pos == NULL) return 0;

    gauche = (pos == ligne) || (est_char_mot(pos[-1]) == 0);
    droite = (est_char_mot(pos[longueur]) == 0);

    if (gauche && droite)
        return 1;
    else
        return 0;
}

/* ============================================
   -w : rechercher un mot entier
   ============================================ */
void ninkam_word(const char *motif, const char *filename) {

    FILE *f;
    char  ligne[max_ligne];

    f = fopen(filename, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, motif) != NULL) {
            if (est_mot_entier(ligne, (char *)motif) == 1)
                printf("%s", ligne);
        }
    }

    fclose(f);
}