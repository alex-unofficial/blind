/* A Program which takes as input a valid image file and prints it
 * on the terminal using braille characters
 */

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
    // initializing the random seed based on current time
    srand(time(0));

    // getting terminal dimensions
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);

    // height is 2 times the terminal rows because each braille character is 
    // treated like 2 'pixels' on top of each other
    int height = 2 * win.ws_row;
    int width = win.ws_col;

    // handling input
    char* filename;
    if(argc == 1) {
        fprintf(stderr, "%s ERROR: You must provide an image file\n", argv[0]);
        fprintf(stderr, "Usage: %s image_file\n", argv[0]);
        exit(1);
    } else {
        filename = argv[1];
    }

    // opening image file and handling read error
    FILE *img;
    img = fopen(filename, "r");
    if(img == NULL) {
        fprintf(stderr, "ERROR: File %s does not exist\n", filename);
        exit(1);
    }

    // the pixels array contains the brightness values in the input image 
    // for every character in the terminal
    double **pixels;
    // allocating memory for the pixels array
    pixels = (double **) malloc(height * sizeof(double *));
    for(int i = 0 ; i < height ; i++) {
        pixels[i] = (double *) malloc(width * sizeof(double));
    }

    // using the input image we construct the pixels array
    GetPixelValues(img, pixels, width, height);

    // initializing the arrays used for printing the braille characters
    initialize_braille();

    setlocale(LC_CTYPE, "");
    
    initscr(); // initialize ncurses
    cbreak();  // disable input buffering
    noecho();  // disable echoing input in the screen

    curs_set(0); // return cursor to beginning of the screen

    clear(); // clear the screen
   
    // for every character in the terminal window
    for(int i = 0 ; i < win.ws_row ; i++) {
        for(int j = 0 ; j < win.ws_col ; j++) {
            // for all characters in every row 2 pixel inputs are accessed,
            // `u` for the upper pixel and `l` for the lower one.
            // each half-braille character can contain from 0 to 4 dots
            // so the normalized array (0..1) is converted to
            // the closest integer approximation from 0 to 4
            int u = dround(4 * pixels[2 * i][j]);
            int l = dround(4 * pixels[2 * i + 1][j]);
            mvprintbraille(i, j, random_braille(u,l));
        }
    }

    char c;
    while(true) {
        // read characters until q is pressed at which point exit the program
        c = getch();
        if(c == 'q') break;
    }
    endwin();

    // deallocate memory for the pixels array
    for(int i = 0 ; i < height ; i++) {
        free(pixels[i]);
    }
    free(pixels);

    // free the memory used in constructing the braille characters
    free_braille();

    return 0;
}
