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
        fprintf(stderr, "%s ERROR: You must provide an image file\n", argv[0]);
        fprintf(stderr, "Usage: %s image_file\n", argv[0]);
        exit(1);
    } else {
        filename = argv[1];
    }

    MagickBooleanType status;
    MagickWand* wand;

    MagickWandGenesis();
    wand = NewMagickWand();

    status = MagickReadImage(wand, filename);
    
    if(status == MagickFalse) {
        ThrowWandException(wand);
    }

    MagickScaleImage(wand, width, height);
    MagickSetImageColorspace(wand, GRAYColorspace);

    double **pixels;
    pixels = (double **) malloc(height * sizeof(double *));
    for(int i = 0 ; i < height ; i++) {
        pixels[i] = (double *) malloc(width * sizeof(double));
    }

    PixelIterator *iterator;

    PixelWand **pixel;
    size_t num_pixel;
    
    iterator = NewPixelIterator(wand);
    if(iterator == (PixelIterator *) NULL) {
        ThrowWandException(wand);
    }

    pixel = PixelGetNextIteratorRow(iterator, &num_pixel);
    for(int i = 0; pixel != (PixelWand **) NULL; i++) {
        for(int j = 0 ; j < num_pixel ; j++) {
            pixels[i][j] = PixelGetBlue(pixel[j]);
        }

        pixel = PixelGetNextIteratorRow(iterator, &num_pixel);   
    }

    wand = DestroyMagickWand(wand);
    MagickWandTerminus();

    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    //here there be curses
    initscr();
    cbreak();
    noecho();

    curs_set(0);

    clear();
   
    // TODO: change below to print the new normalized array
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
