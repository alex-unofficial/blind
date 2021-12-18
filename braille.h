#ifndef BRAILLE_H
#define BRAILLE_H

/* Contains functions to generate and print
 * Unicode braille characters
 */

#include <ncurses.h>
#include <wchar.h>
#include <locale.h>

extern char ***bytes_by_bits; 
// is a 5x5 array of arrays. bytes_by_bits[m][n] contains all the bytes
// whose braille representation contains `m` upper dots and `n` lower dots

extern int **len;	// is a 5x5 array. len[m][n] contains 
									// the length of the bytes_by_bits[m][n] array

/* alloc/dealloc */
void initialize_braille();	
// should be called before any other function
// initializes the `bytes_by_bits` and `len` arrays with the correct values.

void free_braille();	// should be called after all other functions
											// frees the memory allocated to `bytes_by_bits` and `len`.

/* get braille character */
wchar_t get_braille(char a, char b, char c, char d, char e, char f, char g, char h);
// returns a braille characrer defined by a..h representing the individual dots

wchar_t random_braille(int u_dots, int l_dots); 
// returns a random braille character 
// with `u_dots` upper dots and `l_dots` lower dots.

void convert_to_braille(int **dots, wchar_t **braille, int range, size_t rows, size_t cols); 

/* output */
void print_braille(wchar_t braille);	// prints a braille character to `stdout`.

void mvprintbraille(int i, int j, wchar_t braille); 
// with ncurses: moves the cursor to (i,j) and 
// prints the braille character in that position.

#endif
