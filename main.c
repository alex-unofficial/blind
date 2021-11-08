#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include <ncurses.h>
#include <wand/MagickWand.h>

#include "braille.h"
#include "helpers.h"

int main() {
    srand(time(0));

    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    /*-DISPLAY-CODE-GOES-HERE-*/
    clear();

    FILE *f;
    f = fopen("chiaroscuro.gray", "r");

    unsigned char F[90][99];

    /*
    for(int j = 0 ; j < 99; j++) {
        fgetc(f); 
    }
    */

    for(int i = 0 ; i < 90 ; i++) {
        for(int j = 0 ; j < 99; j++) {
            F[i][j] = fgetc(f); 
        }
    }

    // normalizing
    float k[90][99];
    for(int i = 0 ; i < 90 ; i++) {
        for(int j = 0 ; j < 99; j++) {
            k[i][j] = (float)F[i][j]/255;
        }
    }
    
    for(int i = 0 ; i < 45 ; i++) {
        for(int j = 0 ; j < 99 ; j++) {
            int u = fround(4*k[2*i][j]);
            int l = fround(4*k[2*i+1][j]);
            mvprintbraille(i, j, random_braille(u,l));
        }
    }
    /*------------------------*/

    getch();
    endwin();

    free_braille();

    return 0;
}
