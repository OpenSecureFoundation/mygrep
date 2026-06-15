#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define max_ligne 1024

/* Variables globales */
int   opt_color       = 0;    /* --color       */
char *opt_exclude_dir = NULL; /* --exclude-dir */

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

/* ============================================
   --color : coloriser le motif trouvé
   ============================================ */
void ninkam_color(const char *motif, const char *fichier) {
    FILE *f;
    char  ligne[max_ligne];
    int   len = strlen(motif);

    f = fopen(fichier, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, motif) != NULL) {
            char *pos = ligne;
            while (*pos) {
                if (strncmp(pos, motif, len) == 0) {
                    /* Rouge gras */
                    printf("\033[1;31m%s\033[0m", motif);
                    pos += len;
                } else {
                    putchar(*pos);
                    pos++;
                }
            }
        }
    }
    fclose(f);
}

/* ============================================
   --exclude-dir : exclure un dossier
   ============================================ */
void ninkam_exclude_dir(const char *dossier,
                         const char *motif,
                         const char *dir_exclure) {
    DIR           *dir;
    struct dirent *entree;
    struct stat    info;
    char           chemin[max_ligne];

    dir = opendir(dossier);
    if (dir == NULL) {
        perror("Erreur ouverture dossier");
        return;
    }

    while ((entree = readdir(dir)) != NULL) {
        if (strcmp(entree->d_name, ".") == 0 ||
            strcmp(entree->d_name, "..") == 0)
            continue;

        /* Ignorer le dossier exclu */
        if (strcmp(entree->d_name, dir_exclure) == 0)
            continue;

        snprintf(chemin, sizeof(chemin), "%s/%s",
                 dossier, entree->d_name);

        if (stat(chemin, &info) == 0) {
            if (S_ISDIR(info.st_mode))
                ninkam_exclude_dir(chemin, motif, dir_exclure);
            else if (S_ISREG(info.st_mode))
                ninkam_search(motif, chemin);
        }
    }
    closedir(dir);
}