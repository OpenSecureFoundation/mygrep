#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <regex.h>

#define MAX_LIGNE  1024
#define MAX_MOTIFS 10

/* Variables globales */
int opt_recursif      = 0;  /* -r */
int opt_occurrence    = 0;  /* -o */
int opt_ligne_entiere = 0;  /* -x */
int opt_perl_regex    = 0;  /* -P */

char *motifs[MAX_MOTIFS];
int   nb_motifs = 0;

/* ============================================
   -x : vérifier si la ligne entière correspond
   ============================================ */
int correspond_x(const char *ligne, const char *motif) {
    char ligne_propre[MAX_LIGNE];
    strncpy(ligne_propre, ligne, MAX_LIGNE);
    ligne_propre[strcspn(ligne_propre, "\n")] = '\0';
    return strcmp(ligne_propre, motif) == 0;
}

/* ============================================
   -P : vérifier avec une regex
   ============================================ */
int correspond_regex(const char *ligne, const char *motif) {
    regex_t regex;
    int     resultat;

    resultat = regcomp(&regex, motif, REG_EXTENDED);
    if (resultat != 0) {
        fprintf(stderr, "Erreur : motif regex invalide\n");
        return 0;
    }

    resultat = regexec(&regex, ligne, 0, NULL, 0);
    regfree(&regex);
    return resultat == 0;
}

/* ============================================
   Vérifier si la ligne correspond au motif
   ============================================ */
int correspond(const char *ligne) {
    int i;
    for (i = 0; i < nb_motifs; i++) {
        if (opt_perl_regex) {
            if (correspond_regex(ligne, motifs[i]))
                return 1;
        } else if (opt_ligne_entiere) {
            if (correspond_x(ligne, motifs[i]))
                return 1;
        } else {
            if (strstr(ligne, motifs[i]) != NULL)
                return 1;
        }
    }
    return 0;
}

/* ============================================
   Afficher uniquement le motif trouvé (-o)
   ============================================ */
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

/* ============================================
   Rechercher dans un fichier
   ============================================ */
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

/* ============================================
   Recherche récursive (-r)
   ============================================ */
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

/* ============================================
   Fonction principale
   ============================================ */
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
        } else if (strcmp(argv[i], "-x") == 0) {
            opt_ligne_entiere = 1;
        } else if (strcmp(argv[i], "-P") == 0) {
            opt_perl_regex = 1;
        } else {
            break;
        }
    }

    if (nb_motifs == 0 || i >= argc) {
        fprintf(stderr,
            "Usage: %s [-r] [-o] [-x] [-P] -e <motif> <fichier|dossier>\n",
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
