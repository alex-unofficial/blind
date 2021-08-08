#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>

char *bytes_by_bits[5][5];
int len[5][5];

int binom(int n, int k) {
    if(k == 0 || k == n) return 1;
    
    k = (n - k < k)? n - k : k;

    float prod = 1;
    for(int i = 1 ; i <= k ; i++) {
        prod *= (float)(n + 1 - i)/i;
    }

    return (int)prod;
}

int fround(float x) {
    return (int) (2 * x) - (int) x;
}

void print_braille(wchar_t braille) {
    wprintf(L"%lc", braille);
}

wchar_t random_braille(int u_dots, int l_dots) {
    unsigned char offset = bytes_by_bits[u_dots][l_dots][rand() % len[u_dots][l_dots]];
    wchar_t braille = 0x2800 + offset;
    return braille;
}

int main() {
    srand(time(0));

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

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    if(has_colors() == FALSE) {
        endwin();
        wprintf(L"Your terminal does not support color\n");
        exit(1);
    }

    start_color();
    /*-DISPLAY-CODE-GOES-HERE-*/
    init_pair(1, COLOR_CYAN, COLOR_BLUE);

    clear();
    attron(COLOR_PAIR(1));
    for(int y = 0 ; y < LINES ; y++) {
        for(int x = 0 ; x < COLS ; x++) {
            int k = fround(4*((float)x/COLS));
            wchar_t str[] = {random_braille(k,k), L'\0'};
            mvaddwstr(y, x, str);
        }
    }
    attroff(COLOR_PAIR(1));
    /*------------------------*/

    getch();
    endwin();

    
    // deallocating memory
    for(int s = 0 ; s < 5 ; s++) {
        for(int k = 0 ; k < 5 ; k++) {
            free(bytes_by_bits[s][k]);
        }
    }

    return 0;
}
