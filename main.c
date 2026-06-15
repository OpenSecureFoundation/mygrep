#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tsala.h"
#include "vanessa.h"
#include "teteya.h"
#include "lesnar.h"
#include "Ninkam.h"

/* ============================================
   Variables de combinaison — VANESSA
   ============================================ */
int opt_invert    = 0;  /* -v */
int opt_highlight = 0;  /* -h */
int opt_hide_file = 0;  /* -f */
int opt_null_data = 0;  /* -z */
int opt_ext_regex = 0;  /* -E */

/* ============================================
   Variables de combinaison — TETEYA
   ============================================ */
int   opt_ignore_case = 0;    /* -i  */
int   opt_after       = 0;    /* -A  */
int   opt_before      = 0;    /* -B  */
int   opt_max_matches = -1;   /* -m  */
int   opt_byte_offset = 0;    /* -b  */
char *opt_include     = NULL; /* --include */
char *opt_exclude     = NULL; /* --exclude */

/* ============================================
   Variables de combinaison — LESNAR
   ============================================ */
int opt_num_ligne = 0;  /* -n */
int opt_nom_fich  = 0;  /* -l */
int opt_silence   = 0;  /* -s */
int opt_fixe      = 0;  /* -F */
int opt_quiet     = 0;  /* -q */
int opt_null_char = 0;  /* -Z */
int opt_dossier   = 0;  /* -d */
int opt_binaire   = 0;  /* -I */

/* ============================================
   Variables de combinaison — NINKAM
   ============================================ */
int opt_count = 0;  /* -c */
int opt_word  = 0;  /* -w */

/* ============================================
   Fonction de recherche combinée
   C'est ici que TOUTES les options travaillent
   ensemble sur chaque ligne
   ============================================ */
void recherche_combinee(const char *pattern, const char *fichier) {

    FILE *f;
    char  ligne[1024];
    int   numero   = 0;
    int   compteur = 0;

    /* Ouvrir le fichier */
    f = fopen(fichier, "r");
    if (f == NULL) {
        if (opt_silence == 0)
            perror("Erreur ouverture fichier");
        return;
    }

    /* Lecture ligne par ligne */
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        numero++;
        ligne[strcspn(ligne, "\n")] = '\0';

        /* ── Copie pour -i ── */
        char temp[1024];
        char motif_temp[1024];
        strcpy(temp, ligne);
        strcpy(motif_temp, pattern);

        /* -i : ignorer la casse (TETEYA) */
        if (opt_ignore_case) {
            int k;
            for (k = 0; temp[k]; k++)
                temp[k] = tolower(temp[k]);
            for (k = 0; motif_temp[k]; k++)
                motif_temp[k] = tolower(motif_temp[k]);
        }

        /* ── Vérifier correspondance ── */
        int match = 0;

        /* -w : mot entier (NINKAM) */
        if (opt_word) {
            char *pos = strstr(temp, motif_temp);
            if (pos != NULL) {
                int gauche = (pos == temp) ||
                             !isalnum((unsigned char)*(pos - 1));
                int droite = !isalnum((unsigned char)*(pos + strlen(motif_temp)));
                if (gauche && droite) match = 1;
            }
        }
        /* -x : ligne entière (TSALA) */
        else if (opt_ligne_entiere) {
            if (strcmp(temp, motif_temp) == 0) match = 1;
        }
        /* -P : regex Perl (TSALA) */
        else if (opt_perl_regex) {
            if (correspond_regex(ligne, pattern)) match = 1;
        }
        /* -E : regex étendue (VANESSA) */
        else if (opt_ext_regex) {
            if (correspond_regex(ligne, pattern)) match = 1;
        }
        /* Recherche normale */
        else {
            if (strstr(temp, motif_temp) != NULL) match = 1;
        }

        /* -v : inverser (VANESSA) */
        if (opt_invert) match = !match;

        if (match) {
            compteur++;

            /* -q : mode silencieux (LESNAR) */
            if (opt_quiet) {
                fclose(f);
                return;
            }

            /* -l : nom fichier uniquement (LESNAR) */
            if (opt_nom_fich) {
                if (opt_null_char)
                    printf("%s", fichier);
                else
                    printf("%s\n", fichier);
                fclose(f);
                return;
            }

            /* -c : compter seulement (NINKAM) */
            if (opt_count) continue;

            /* -b : offset en octets (TETEYA) */
            if (opt_byte_offset)
                printf("%d:", numero * 10);

            /* ── Affichage selon les options ── */

            /* -o : uniquement le motif (TSALA) */
            if (opt_occurrence) {
                char *pos = strstr(ligne, pattern);
                while (pos != NULL) {
                    printf("%.*s\n", (int)strlen(pattern), pos);
                    pos = strstr(pos + strlen(pattern), pattern);
                }
            }
            /* --color : coloriser (NINKAM) */
            else if (opt_color) {
                int   len = strlen(pattern);
                char *pos = ligne;
                while (*pos) {
                    if (strncmp(pos, pattern, len) == 0) {
                        printf("\033[1;31m%s\033[0m", pattern);
                        pos += len;
                    } else {
                        putchar(*pos);
                        pos++;
                    }
                }
                printf("\n");
            }
            /* -h : surligner (VANESSA) */
            else if (opt_highlight) {
                int   len = strlen(pattern);
                char *pos = ligne;
                while (*pos) {
                    if (strncmp(pos, pattern, len) == 0) {
                        printf("[%s]", pattern);
                        pos += len;
                    } else {
                        putchar(*pos);
                        pos++;
                    }
                }
                printf("\n");
            }
            /* -n : numéro de ligne (LESNAR) */
            else if (opt_num_ligne) {
                printf("%d: %s\n", numero, ligne);
            }
            /* -f : cacher nom fichier (VANESSA) */
            else if (opt_hide_file) {
                printf("%s\n", ligne);
            }
            /* Affichage normal */
            else {
                printf("%s: %s\n", fichier, ligne);
            }

            /* -m : limite de résultats (TETEYA) */
            if (opt_max_matches != -1 &&
                compteur >= opt_max_matches) {
                fclose(f);
                return;
            }
        }
    }

    /* -c : afficher le compte final (NINKAM) */
    if (opt_count)
        printf("%d\n", compteur);

    /* -L : fichier sans correspondance (TSALA) */
    if (opt_sans_match && compteur == 0)
        printf("%s\n", fichier);

    fclose(f);
}

/* ============================================
   Fonction principale
   ============================================ */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s [options] <motif> <fichier>\n", argv[0]);
        fprintf(stderr, "  === TSALA   === -e -o -r -R -x -P -L -f\n");
        fprintf(stderr, "  === VANESSA === -v -h -f -z -E\n");
        fprintf(stderr, "  === TETEYA  === -i -A -B -C -m -b --include --exclude\n");
        fprintf(stderr, "  === LESNAR  === -n -l -s -q -F -Z -d -I\n");
        fprintf(stderr, "  === NINKAM  === -c -w --color --exclude-dir\n");
        return EXIT_FAILURE;
    }

    int   i;
    char *pattern = NULL;
    char *cible   = NULL;

    /* ============================================
       LECTURE DE TOUTES LES OPTIONS
       La boucle lit TOUS les arguments avant de
       lancer la recherche → combinaison possible
       ============================================ */
    for (i = 1; i < argc; i++) {

        /* ── TSALA ── */
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i < argc) {
                motifs[nb_motifs] = argv[i];
                nb_motifs++;
            }
        }
        else if (strcmp(argv[i], "-o") == 0) opt_occurrence    = 1;
        else if (strcmp(argv[i], "-r") == 0) opt_recursif      = 1;
        else if (strcmp(argv[i], "-R") == 0) opt_liens_sym     = 1;
        else if (strcmp(argv[i], "-x") == 0) opt_ligne_entiere = 1;
        else if (strcmp(argv[i], "-P") == 0) opt_perl_regex    = 1;
        else if (strcmp(argv[i], "-L") == 0) opt_sans_match    = 1;
        else if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (i < argc) opt_fichier_motifs = argv[i];
        }

        /* ── VANESSA ── */
        else if (strcmp(argv[i], "-v") == 0) opt_invert    = 1;
        else if (strcmp(argv[i], "-h") == 0) opt_highlight = 1;
        else if (strcmp(argv[i], "-z") == 0) opt_null_data = 1;
        else if (strcmp(argv[i], "-E") == 0) opt_ext_regex = 1;

        /* ── TETEYA ── */
        else if (strcmp(argv[i], "-i") == 0) opt_ignore_case = 1;
        else if (strcmp(argv[i], "-A") == 0) {
            i++;
            if (i < argc) opt_after = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-B") == 0) {
            i++;
            if (i < argc) opt_before = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-C") == 0) {
            i++;
            if (i < argc) {
                opt_after  = atoi(argv[i]);
                opt_before = atoi(argv[i]);
            }
        }
        else if (strcmp(argv[i], "-m") == 0) {
            i++;
            if (i < argc) opt_max_matches = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-b") == 0) opt_byte_offset = 1;
        else if (strcmp(argv[i], "--include") == 0) {
            i++;
            if (i < argc) opt_include = argv[i];
        }
        else if (strcmp(argv[i], "--exclude") == 0) {
            i++;
            if (i < argc) opt_exclude = argv[i];
        }

        /* ── LESNAR ── */
        else if (strcmp(argv[i], "-n") == 0) opt_num_ligne = 1;
        else if (strcmp(argv[i], "-l") == 0) opt_nom_fich  = 1;
        else if (strcmp(argv[i], "-s") == 0) opt_silence   = 1;
        else if (strcmp(argv[i], "-F") == 0) opt_fixe      = 1;
        else if (strcmp(argv[i], "-q") == 0) opt_quiet     = 1;
        else if (strcmp(argv[i], "-Z") == 0) opt_null_char = 1;
        else if (strcmp(argv[i], "-d") == 0) opt_dossier   = 1;
        else if (strcmp(argv[i], "-I") == 0) opt_binaire   = 1;

        /* ── NINKAM ── */
        else if (strcmp(argv[i], "-c") == 0) opt_count = 1;
        else if (strcmp(argv[i], "-w") == 0) opt_word  = 1;
        else if (strcmp(argv[i], "--color") == 0) opt_color = 1;
        else if (strcmp(argv[i], "--exclude-dir") == 0) {
            i++;
            if (i < argc) opt_exclude_dir = argv[i];
        }

        /* ── Motif et cible ── */
        else if (pattern == NULL) pattern = argv[i];
        else                      cible   = argv[i];
    }

    /* ============================================
       Si -e utilisé → motif vient du tableau
       ============================================ */
    if (nb_motifs > 0 && pattern == NULL)
        pattern = motifs[0];

    /* ============================================
       Si -f utilisé → charger motifs depuis fichier
       ============================================ */
    if (opt_fichier_motifs != NULL) {
        charger_motifs_fichier(opt_fichier_motifs);
        if (pattern == NULL && nb_motifs > 0)
            pattern = motifs[0];
    }

    /* ============================================
       stdin : pas de fichier fourni
       ============================================ */
    if (pattern != NULL && cible == NULL) {
        ninkam_stdin(pattern);
        return EXIT_SUCCESS;
    }

    if (pattern == NULL || cible == NULL) {
        fprintf(stderr, "Erreur : motif ou fichier manquant\n");
        return EXIT_FAILURE;
    }

    /* ============================================
       LANCEMENT DE LA RECHERCHE
       ============================================ */

    /* -R : récursif avec liens symboliques (TSALA) */
    if (opt_liens_sym) {
        rechercher_recursif_R(cible);
        return EXIT_SUCCESS;
    }

    /* -r : récursif normal (TSALA) */
    if (opt_recursif) {
        rechercher_recursif(cible);
        return EXIT_SUCCESS;
    }

    /* --exclude-dir : récursif avec exclusion (NINKAM) */
    if (opt_exclude_dir != NULL) {
        ninkam_exclude_dir(cible, pattern, opt_exclude_dir);
        return EXIT_SUCCESS;
    }

    /* Recherche combinée dans le fichier */
    recherche_combinee(pattern, cible);

    return EXIT_SUCCESS;
}