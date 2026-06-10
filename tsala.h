
#ifndef TSALA_H
#define TSALA_H

#include <stdio.h>

extern int   opt_recursif;
extern int   opt_occurrence;
extern int   opt_ligne_entiere;
extern int   opt_perl_regex;
extern char *motifs[10];
extern int   nb_motifs;

int  correspond(const char *ligne);
void afficher_occurrence(const char *ligne);
void rechercher_fichier(const char *fichier);
void rechercher_recursif(const char *dossier);

#endif

