#ifndef LESNAR_H
#define LESNAR_H

#include <stdio.h>

/* Flags */
extern int flag_n;
extern int flag_l;
extern int flag_s;
extern int flag_F;
extern int flag_q;
extern int flag_Z;
extern int flag_d;
extern int flag_I;

/* Fonctions */
void flag_Z_function(char *filename);
int  flag_l_function(int flag_Z, char *filename, FILE *file);
void flag_n_function(int n, char *line);
int  flag_q_function(FILE *file);
int  flag_F_function(char *line, FILE *file);
int  flag_I_function(FILE *file);
int  flag_d_function(int argc, char *argv[]);
void lesnar_search(int argc, char *argv[]);

#endif