//#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include <sys/ioctl.h>
#include <unistd.h>

#include <ncurses.h>
#include <MagickWand/MagickWand.h>

#include "braille.h"
#include "magick.h"
#include "helpers.h"

int main(int argc, char** argv) {
    srand(time(0));

    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    int height = 2 * win.ws_row;
    int width = win.ws_col;

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

    MagickBooleanType status;
    MagickWand* wand;

    MagickWandGenesis();
    wand = NewMagickWand();

    status = MagickReadImage(wand, filename);
    
    if(status == MagickFalse) {
        ThrowWandException(wand);
    }

    // TODO: resize image to terminal size
    // TODO: make image black and white
    // TODO: get pixel values into a 2D array

    wand = DestroyMagickWand(wand);
    MagickWandTerminus();

    // TODO: normalize the array

    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    clear();
   
    // TODO: change below to print the new normalized array
    for(int i = 0 ; i < 45 ; i++) {
        for(int j = 0 ; j < 99 ; j++) {
            int u = fround(4*k[2*i][j]);
            int l = fround(4*k[2*i+1][j]);
            mvprintbraille(i, j, random_braille(u,l));
        }
    }

    getch();
    endwin();

    free_braille();

    return 0;
}
