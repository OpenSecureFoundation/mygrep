#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LIGNE  1024
#define MAX_MOTIFS 10

/* Variables globales */
int opt_recursif   = 0;
int opt_occurrence = 0;
char *motifs[MAX_MOTIFS];
int   nb_motifs = 0;

/* Cherche le motif dans une ligne */
int correspond(const char *ligne) {
    int i;
    for (i = 0; i < nb_motifs; i++) {
        if (strstr(ligne, motifs[i]) != NULL)
            return 1;
    }
    return 0;
}

/* Affiche uniquement le motif trouvé (-o) */
void afficher_occurrence(const char *ligne) {
    int i;
    for (i = 0; i < nb_motifs; i++) {
        char *pos = strstr(ligne, motifs[i]);
        while (pos != NULL) {
            printf("%.*s\n", (int)strlen(motifs[i]), pos);
            pos = strstr(pos + strlen(motifs[i]), motifs[i]);
        }
    }
}

/* Recherche dans un fichier */
void rechercher_fichier(const char *fichier) {
    FILE *f;
    char  ligne[MAX_LIGNE];

    f = fopen(fichier, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        ligne[strcspn(ligne, "\n")] = '\0';
        if (correspond(ligne)) {
            if (opt_occurrence)
                afficher_occurrence(ligne);
            else
                printf("%s: %s\n", fichier, ligne);
        }
    }
    fclose(f);
}

/* Recherche recursive dans un dossier (-r) */
void rechercher_recursif(const char *dossier) {
    DIR           *dir;
    struct dirent *entree;
    struct stat    info;
    char           chemin[MAX_LIGNE];

    dir = opendir(dossier);
    if (dir == NULL) {
        perror("Erreur ouverture dossier");
        return;
    }

    while ((entree = readdir(dir)) != NULL) {
        if (strcmp(entree->d_name, ".") == 0 ||
            strcmp(entree->d_name, "..") == 0)
            continue;

        snprintf(chemin, sizeof(chemin), "%s/%s",
                 dossier, entree->d_name);

        if (stat(chemin, &info) == 0) {
            if (S_ISDIR(info.st_mode))
                rechercher_recursif(chemin);
            else if (S_ISREG(info.st_mode))
                rechercher_fichier(chemin);
        }
    }
    closedir(dir);
}

/* Fonction principale */
int main(int argc, char *argv[]) {
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i < argc) {
                motifs[nb_motifs] = argv[i];
                nb_motifs++;
            }
        } else if (strcmp(argv[i], "-r") == 0) {
            opt_recursif = 1;
        } else if (strcmp(argv[i], "-o") == 0) {
            opt_occurrence = 1;
        } else {
            break;
        }
    }

    if (nb_motifs == 0 || i >= argc) {
        fprintf(stderr,
            "Usage: %s [-r] [-o] -e <motif> <fichier|dossier>\n",
            argv[0]);
        return EXIT_FAILURE;
    }

    char *cible = argv[i];

    if (opt_recursif)
        rechercher_recursif(cible);
    else
        rechercher_fichier(cible);

    return EXIT_SUCCESS;
}