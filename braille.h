#ifndef BRAILLE_H
#define BRAILLE_H

#include <ncurses.h>
#include <wchar.h>
#include <locale.h>

char *bytes_by_bits[5][5];
int len[5][5];

// alloc/dealloc
void initialize_braille();
void free_braille();

// get braille character
wchar_t random_braille(int u_dots, int l_dots);

// output
void print_braille(wchar_t braille);
void mvprintbraille(int i, int j, wchar_t braille);

#endif
