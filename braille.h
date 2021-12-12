#ifndef BRAILLE_H
#define BRAILLE_H

#include <ncurses.h>
#include <wchar.h>
#include <locale.h>

extern char ***bytes_by_bits;
extern int **len;

// alloc/dealloc
void initialize_braille(); //should be called before any other function
void free_braille();			 //should be called after all other functions

// get braille character
wchar_t random_braille(int u_dots, int l_dots);

// output
void print_braille(wchar_t braille);
void mvprintbraille(int i, int j, wchar_t braille);

#endif
