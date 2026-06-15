#ifndef NINKAM_H
#define NINKAM_H

#include <stdio.h>

/* grep_basic.c */
void ninkam_search(const char *motif, const char *filename);

/* optionc.c */
void ninkam_count(const char *motif, const char *filename);

/* optionw.c */
int  est_char_mot(char c);
int  est_mot_entier(char *ligne, char *motif);
void ninkam_word(const char *motif, const char *filename);

/* option_search_stdin.c */
void ninkam_stdin(const char *motif);

#endif