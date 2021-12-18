/* A Program which takes as input a valid image file and prints it
 * on the terminal using braille characters
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    
    // some options
    int range = 1;
    int dither = true;

    // getting terminal dimensions
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    int height = 4 * win.ws_row / sqrt(range);
    int width = 2 * win.ws_col / sqrt(range);

    // the pixels array contains the brightness values in the input image 
    // for every character in the terminal
    double **pixels;
    // the dots array contains the ammount of dots to use per "pixel"
    int **dots;
    // allocating memory for the arrays
    alloc2d(pixels, double, height, width);
    alloc2d(dots, int, height, width);

    // using the input image we construct the pixels array
    GetPixelValues(img, pixels, width, height);

    // quantize and apply dithering
    quantize_image(pixels, dots, range, width, height, dither);

    // the braille array contains the braille characters to print to the screen
    wchar_t **braille;
    alloc2d(braille, wchar_t, win.ws_row, win.ws_col);

    // converting to braille array
    convert_to_braille(dots, braille, range, win.ws_row, win.ws_col);

    // pixels and dots are no longer needed so they are freed
    free2d(pixels, height);
    free2d(dots, height);

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
            mvprintbraille(i, j, braille[i][j]);
        }
    }

    int c;
    while(true) {
        // read characters until q is pressed at which point exit the program
        c = getch();
        if(c == 'q' || c == EOF) break;


    }
    endwin();

    // deallocate memory for the braille array
    free2d(braille, win.ws_row);

    // free the memory used in constructing the braille characters
    free_braille();

    return 0;
}
