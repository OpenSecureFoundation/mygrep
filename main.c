#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Headers de chaque membre */
#include "tsala.h"
#include "vanessa.h"
#include "teteya.h"
#include "lesnar.h"
#include "Ninkam.h"

int main(int argc, char *argv[]) {

    /* Vérification arguments minimum */
    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s [options] <motif> <fichier|dossier>\n",
            argv[0]);
        fprintf(stderr, "Options disponibles :\n");
        fprintf(stderr, "  === TSALA ===\n");
        fprintf(stderr, "  -e  : motif de recherche\n");
        fprintf(stderr, "  -o  : afficher uniquement le motif\n");
        fprintf(stderr, "  -r  : recherche recursive\n");
        fprintf(stderr, "  -x  : ligne entiere\n");
        fprintf(stderr, "  -P  : regex Perl\n");
        fprintf(stderr, "  === VANESSA ===\n");
        fprintf(stderr, "  -v  : inverser la recherche\n");
        fprintf(stderr, "  -h  : surligner le motif\n");
        fprintf(stderr, "  -f  : cacher le nom du fichier\n");
        fprintf(stderr, "  -z  : recherche donnees nulles\n");
        fprintf(stderr, "  -E  : regex etendue\n");
        fprintf(stderr, "  === TETEYA ===\n");
        fprintf(stderr, "  -i  : ignorer la casse\n");
        fprintf(stderr, "  -A  : lignes apres\n");
        fprintf(stderr, "  -B  : lignes avant\n");
        fprintf(stderr, "  -C  : lignes autour\n");
        fprintf(stderr, "  -m  : limite resultats\n");
        fprintf(stderr, "  -b  : offset en octets\n");
        fprintf(stderr, "  === LESNAR ===\n");
        fprintf(stderr, "  -n  : numero de ligne\n");
        fprintf(stderr, "  -l  : nom du fichier\n");
        fprintf(stderr, "  -s  : supprimer erreurs\n");
        fprintf(stderr, "  -q  : mode silencieux\n");
        fprintf(stderr, "  -F  : chaine fixe\n");
        fprintf(stderr, "  === NINKAM ===\n");
        fprintf(stderr, "  -c  : compter les lignes correspondantes\n");
        fprintf(stderr, "  -w  : rechercher un mot entier\n");
        fprintf(stderr, "  (stdin) : lecture depuis l'entree standard\n");
        fprintf(stderr, "  (basic) : recherche simple sans option\n");
        return EXIT_FAILURE;
    }

    /* ============================================
       TSALA : options -e -r -o -x -P
       ============================================ */
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

    /* Si motifs trouvés → lancer fonctions Tsala */
    if (nb_motifs > 0) {
        if (opt_recursif)
            rechercher_recursif(argv[argc - 1]);
        else
            rechercher_fichier(argv[argc - 1]);
        return EXIT_SUCCESS;
    }

    /* ============================================
       Récupération flag et pattern
       ============================================ */
    char *flag    = argv[1];
    char *pattern = argv[2];
    char *cible   = argv[argc - 1];

    /* ============================================
       NINKAM : stdin - pas de fichier
       ============================================ */
    if (argc == 2) {
        ninkam_stdin(argv[1]);
        return EXIT_SUCCESS;
    }

    /* ============================================
       NINKAM : -c compter les lignes
       ============================================ */
    if (strcmp(flag, "-c") == 0) {
        ninkam_count(pattern, cible);
        return EXIT_SUCCESS;
    }

    /* ============================================
       NINKAM : -w mot entier
       ============================================ */
    if (strcmp(flag, "-w") == 0) {
        ninkam_word(pattern, cible);
        return EXIT_SUCCESS;
    }

    /* ============================================
       VANESSA : -r recherche recursive
       ============================================ */
    if (strcmp(flag, "-r") == 0) {
        recursive_search(cible, pattern);
        return EXIT_SUCCESS;
    }

    /* ============================================
       VANESSA : -l label
       ============================================ */
    if (strcmp(flag, "-l") == 0 && argc == 5) {
        FILE *f = fopen(argv[3], "r");
        if (!f) { perror("Erreur fichier"); return EXIT_FAILURE; }
        label(argv[4], pattern, f);
        fclose(f);
        return EXIT_SUCCESS;
    }

    /* ============================================
       VANESSA : -z null data
       ============================================ */
    if (strcmp(flag, "-z") == 0) {
        FILE *f = fopen(cible, "rb");
        if (!f) { perror("Erreur fichier"); return EXIT_FAILURE; }
        null_data(pattern, f);
        fclose(f);
        return EXIT_SUCCESS;
    }

    /* ============================================
       TETEYA : -i -A -B -C -m -b --include --exclude
       ============================================ */
    if (strcmp(flag, "-i")        == 0 ||
        strcmp(flag, "-A")        == 0 ||
        strcmp(flag, "-B")        == 0 ||
        strcmp(flag, "-C")        == 0 ||
        strcmp(flag, "-m")        == 0 ||
        strcmp(flag, "-b")        == 0 ||
        strcmp(flag, "--include") == 0 ||
        strcmp(flag, "--exclude") == 0) {
        teteya_search(argc, argv);
        return EXIT_SUCCESS;
    }

    /* ============================================
       LESNAR : -n -s -F -q -Z -d -I
       ============================================ */
    if (strcmp(flag, "-n") == 0 ||
        strcmp(flag, "-s") == 0 ||
        strcmp(flag, "-F") == 0 ||
        strcmp(flag, "-q") == 0 ||
        strcmp(flag, "-Z") == 0 ||
        strcmp(flag, "-d") == 0 ||
        strcmp(flag, "-I") == 0) {
        lesnar_search(argc, argv);
        return EXIT_SUCCESS;
    }

    /* ============================================
       VANESSA : -v -h -f -c -E
       ============================================ */
    FILE *f = fopen(cible, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return EXIT_FAILURE;
    }

    if      (strcmp(flag, "-v") == 0) invert_match(pattern, f);
    else if (strcmp(flag, "-h") == 0) highlight(pattern, f);
    else if (strcmp(flag, "-f") == 0) hide_filename(pattern, f);
    else if (strcmp(flag, "-E") == 0) extended_regex(pattern, f);

    /* ============================================
       NINKAM : recherche simple sans option
       ============================================ */
    else {
        fclose(f);
        ninkam_search(pattern, cible);
        return EXIT_SUCCESS;
    }

    fclose(f);
    return EXIT_SUCCESS;
}