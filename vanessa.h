
#ifndef VANESSA_H
#define VANESSA_H

#include <stdio.h>

void invert_match(const char *pattern, FILE *f);
void highlight(const char *pattern, FILE *f);
void hide_filename(const char *pattern, FILE *f);
void label(const char *lbl, const char *pattern, FILE *f);
void null_data(const char *pattern, FILE *f);
void recursive_search(const char *dirname, const char *pattern);
void no_ignore_case(const char *pattern, FILE *f);
void extended_regex(const char *pattern, FILE *f);

#endif
