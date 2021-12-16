#ifndef MAGICK_H
#define MAGICK_H

/* Contains functions to assist with handling the MagickWand library
 */

void GetPixelValues(FILE* img, double **pixels, const size_t width, const size_t height);
// assigns to pixels, a width x height array of floats, the brightness value
// at each pixel in the given image.

#endif
