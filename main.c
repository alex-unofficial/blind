//#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include <ncurses.h>
#include <MagickWand/MagickWand.h>

#include "braille.h"
#include "helpers.h"

int main(int argc, char** argv) {
    srand(time(0));

    char* filename;

    if(argc == 1) {
        printf("ERROR: You must provide an image file to show \n");
        printf(" - Usage: %s image_file\n", argv[0]);
        exit(1);
    } else {
        if(is_image(argv[1])) {
            filename = argv[1];
        } else {
            printf("ERROR: Image must be of type '.png' or '.jpg'\n");
            exit(1);
        }
    }

    FILE *f = fopen(filename, "r");
    
    if(f == NULL) {
        printf("file %s does not exist \n", filename);
        exit(1);
    }

    unsigned char F[90][99];

    for(int i = 0 ; i < 90 ; i++) {
        for(int j = 0 ; j < 99; j++) {
            F[i][j] = fgetc(f); 
        }
    }

    fclose(f);

    // normalizing
    float k[90][99];
    for(int i = 0 ; i < 90 ; i++) {
        for(int j = 0 ; j < 99; j++) {
            k[i][j] = (float)F[i][j]/255;
        }
    }

    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    /*-DISPLAY-CODE-GOES-HERE-*/
    clear();
   
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
