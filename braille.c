#include <stdio.h>
#include <stdlib.h>

#include "braille.h"
#include "helpers.h"

char ***bytes_by_bits;
int **len;

/* This function is responsible for allocating the memory and assigning the correct
 * values to the `bytes_by_bits` and `len` arrays.
 *
 * for any braille character:
 *   :: } these are the upper dots
 *   :: } these are the lower dots
 *
 * both the upper dots and lower dots can have 0,1,2,3 or 4 dots each.
 * hence bytes_by_bits[m][n] where m,n<=5 will contain a list of characters
 * whose braille representation (see below) has m upper dots and n lower dots
 *
 * In unicode braille is represented by the values from U+2800 to U+28FF
 * which is to say U+2800 + some offset byte.
 *
 * the offset byte is what is contained in the bytes_by_bits array
 * and the corellation between bytes and braille is like so:
 *
 * 12345678 - where each position is either 0 or 1,
 *
 * the corresponding braille character is:
 *  1 4
 *  2 5
 *  3 6
 *  7 8
 *
 * so if we add the bits in positions 1,2,4,5 and 3,6,7,8 you get 
 * the upper and lower dots respectively for any given byte.
 *
 * ofcourse multiple braille characters (and therefore bytes) have m upper and n lower
 * dots, and the exact amount varies depending on m and n.
 *
 * Each subdivision of dots has 4 positions which can be either 0 or 1,
 * the number of combinations (given by combinatronics) where the number of 1s in
 * all positions is N is given by the binomial coefficient (4, N)
 *
 * then for M upper dots and N lower dots the amount of combinations is equal to the
 * product of the individual combinations.
 *
 * The number of combinations for bytes_by_bits[m][n] is stored in len[m][n]
 *
 * see:
 * https://en.wikipedia.org/wiki/Braille_Patterns
 * https://en.wikipedia.org/wiki/Binomial_coefficient
 */
void initialize_braille() {
    // allocating memory for pointers as described
    bytes_by_bits = (char ***) malloc(5 * sizeof(char **));
    len = (int **) malloc(5 * sizeof(int *));

    for(int s = 0 ; s < 5 ; s++) {
        bytes_by_bits[s] = (char **) malloc(5 * sizeof(char *));
        len[s] = (int *) malloc(5 * sizeof(int));

        for(int k = 0 ; k < 5 ; k++) {
            bytes_by_bits[s][k] = (char *) malloc(binom(4, s) * binom(4, k));
            len[s][k] = 0; // length starts at 0 because it is used as an index below
        }
    }

    // we iterate over all bytes
    for(int i = 0 ; i < 256 ; i++) {
        char bits[8];
        unsigned char j = i;

        // get the bit values at each position on the byte
        for(int b = 0 ; b < 8 ; b++)  {
            bits[b] = (j >> (7 - b)) & 0x01;
        }

        // we add the bit values in the upper and lower positions
        // (the positions are numbered 76543210 in this case)
        int u_sum = bits[3] + bits[4] + bits[6] + bits[7];
        int l_sum = bits[0] + bits[1] + bits[2] + bits[5];

        // then add the byte to the correct position in the array
        bytes_by_bits[u_sum][l_sum][len[u_sum][l_sum]] = i;
        len[u_sum][l_sum]++;
    }
}

/* This function simply deallocates the memory in the 
 * `bytes_by_bits` and `len` arrays
 */
void free_braille() {
    for(int s = 0 ; s < 5 ; s++) {
        for(int k = 0 ; k < 5 ; k++) {
            free(bytes_by_bits[s][k]);
        }
        free(bytes_by_bits[s]);
        free(len[s]);
    }
    free(bytes_by_bits);
    free(len);
}

/* This function returns the unicode character that represents
 * a random braille character with `u_dots` upper and `l_dots` lower dots
 *
 * It simply chooses a random byte from bytes_by_bits[u_dots][l_dots]
 * and offsets U+2800 by it.
 */
wchar_t random_braille(int u_dots, int l_dots) {
    unsigned char offset = bytes_by_bits[u_dots][l_dots][rand() % len[u_dots][l_dots]];
    wchar_t braille = 0x2800 + offset;
    return braille;
}

// correctly prints a braille character to stdout
void print_braille(wchar_t braille) {
    wprintf(L"%lc", braille);
}

// moves the ncurses cursor to position (i,j) then
// prints the braille character correctly
void mvprintbraille(int i, int j, wchar_t braille) {
    wchar_t str[] = {braille, L'\0'};
    mvaddwstr(i, j, str);
}
