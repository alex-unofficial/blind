#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

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
        fprintf(stderr, "%s ERROR: You must provide an image file\n", argv[0]);
        fprintf(stderr, "Usage: %s image_file\n", argv[0]);
        exit(1);
    } else {
        filename = argv[1];
    }

    FILE *img;
    img = fopen(filename, "r");
    if(img == NULL) {
        fprintf(stderr, "ERROR: File %s does not exist\n", filename);
        exit(1);
    }

    double **pixels;
    pixels = (double **) malloc(height * sizeof(double *));
    for(int i = 0 ; i < height ; i++) {
        pixels[i] = (double *) malloc(width * sizeof(double));
    }

    GetPixelValues(img, pixels, width, height);

    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    clear();
   
    for(int i = 0 ; i < win.ws_row ; i++) {
        for(int j = 0 ; j < win.ws_col ; j++) {
            int u = dround(4 * pixels[2 * i][j]);
            int l = dround(4 * pixels[2 * i + 1][j]);
            mvprintbraille(i, j, random_braille(u,l));
        }
    }

    getch();
    endwin();

    for(int i = 0 ; i < height ; i++) {
        free(pixels[i]);
    }
    free(pixels);

    free_braille();

    return 0;
}
