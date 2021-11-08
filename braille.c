#include <stdio.h>
#include <stdlib.h>

#include "braille.h"
#include "helpers.h"

void initialize_braille() {
    // initializing pointers
    for(int s = 0 ; s < 5 ; s++) {
        for(int k = 0 ; k < 5 ; k++) {
            bytes_by_bits[s][k] = (char *) malloc(binom(4, s) * binom(4, k));
            len[s][k] = 0;
        }
    }

    // preparing the bytes_by_bits array
    for(int i = 0 ; i < 256 ; i++) {
        char bits[8];
        unsigned char j = i;
        for(int b = 0 ; b < 8 ; b++)  {
            bits[b] = (j >> (7 - b)) & 0x01;
        }

        int u_sum = bits[3] + bits[4] + bits[6] + bits[7];
        int l_sum = bits[0] + bits[1] + bits[2] + bits[5];

        bytes_by_bits[u_sum][l_sum][len[u_sum][l_sum]] = i;
        len[u_sum][l_sum]++;
    }
}

void free_braille() {
    // deallocating memory
    for(int s = 0 ; s < 5 ; s++) {
        for(int k = 0 ; k < 5 ; k++) {
            free(bytes_by_bits[s][k]);
        }
    }
}

wchar_t random_braille(int u_dots, int l_dots) {
    unsigned char offset = bytes_by_bits[u_dots][l_dots][rand() % len[u_dots][l_dots]];
    wchar_t braille = 0x2800 + offset;
    return braille;
}

void print_braille(wchar_t braille) {
    wprintf(L"%lc", braille);
}

void mvprintbraille(int i, int j, wchar_t braille) {
    wchar_t str[] = {braille, L'\0'};
    mvaddwstr(i, j, str);
}
