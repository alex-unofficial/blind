#ifndef HELPERS_H
#define HELPERS_H

/* Contains a few helper functions
 * that are used throughout the program for various
 * miscellaneous purposes
 */

#include <stdbool.h>

/* macros for allocating and freeing 2d arrays*/
#define alloc2d(p, type, rows, cols) {                  \
    p = (type **) malloc(rows * sizeof(type *));        \
    for(int i = 0 ; i < rows ; i++) {                   \
        p[i] = (type *) malloc(cols * sizeof(type));    \
    }                                                   \
}

#define free2d(p, rows) {               \
    for(int i = 0 ; i < rows ; i++) {   \
        free(p[i]);                     \
    }                                   \
    free(p);                            \
}

int binom(int n, int k); // the binomial coefficient (n, k)
int dround(double x);	// rounds the floating point number to the closest integer

#endif
