#ifndef NINKAM_H
#define NINKAM_H

#include <stdio.h>

/* Variables globales */
extern int   opt_color;
extern char *opt_exclude_dir;

/* grep_basic.c */
void ninkam_search(const char *motif, const char *filename);
void ninkam_color(const char *motif, const char *fichier);
void ninkam_exclude_dir(const char *dossier,
                         const char *motif,
                         const char *dir_exclure);

/* optionc.c */
void ninkam_count(const char *motif, const char *filename);

/* optionw.c */
int  est_char_mot(char c);
int  est_mot_entier(char *ligne, char *motif);
void ninkam_word(const char *motif, const char *filename);

/* option_search_stdin.c */
void ninkam_stdin(const char *motif);

#endif