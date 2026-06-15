#ifndef TSALA_H
#define TSALA_H

#include <stdio.h>

/* Variables globales */
extern int   opt_recursif;
extern int   opt_occurrence;
extern int   opt_ligne_entiere;
extern int   opt_perl_regex;
extern int   opt_sans_match;
extern int   opt_liens_sym;
extern char *opt_fichier_motifs;
extern char *motifs[10];
extern int   nb_motifs;

/* Fonctions */
int  correspond_x(const char *ligne, const char *motif);
int  correspond_regex(const char *ligne, const char *motif);
int  correspond(const char *ligne);
void afficher_occurrence(const char *ligne);
void rechercher_fichier(const char *fichier);
void rechercher_recursif(const char *dossier);
void rechercher_fichier_L(const char *fichier);
void rechercher_recursif_R(const char *dossier);
void charger_motifs_fichier(const char *fichier_motifs);

#endif