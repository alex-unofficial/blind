#include <stdlib.h>

#include <MagickWand/MagickWand.h>

#include "magick.h"
#include "helpers.h"

// handles output of the exceptions from magick wand
#define ThrowWandException(wand) { \
    char *description; \
    ExceptionType severity; \
    \
    description = MagickGetException(wand, &severity); \
    (void) fprintf(stderr, "%s %s %lu %s \n", GetMagickModule(), description); \
    description = (char *) MagickRelinquishMemory(description); \
    exit(-1); \
}

/* This functions takes as input a file pointer to an image file and an allocated
 * `pixels` array to contain the brightness values (along with the width and height).
 *
 * It then utilizes MagickWand utilities to convert the image to greyscale, 
 * resize it to the correct dimensions and then get the pixel brightness values for
 * every pixel in the new image
 */
void GetPixelValues(FILE* img, double **pixels, const size_t width, const size_t height) {
    // initialize the MagickWand utilities and variables
    MagickBooleanType status;
    MagickWand* wand;
    PixelWand* bg;

    MagickWandGenesis();
    wand = NewMagickWand();

    // set the background color to black
    bg = NewPixelWand();
    PixelSetColor(bg, "black");
    MagickSetBackgroundColor(wand, bg);

    // read the image given and throw exception if errors occur
    status = MagickReadImageFile(wand, img);
    if(status == MagickFalse) {
        ThrowWandException(wand);
    }

    // convert to greyscale
    MagickSetImageColorspace(wand, GRAYColorspace);

    // resize the image preserving width:height ratio
    size_t imgWidth, imgHeight;
    imgWidth = MagickGetImageWidth(wand);
    imgHeight = MagickGetImageHeight(wand);

    double width_scalar, height_scalar;
    ssize_t offset = 0;
    
    // the scalars represent how much to scale width and height 
    // to get the desired ones respectively.
    width_scalar = (double)width/imgWidth;
    height_scalar = (double)height/imgHeight;

    // we scale by the minimum of the 2 so both dimensions fit in the new picture
    // then we extend the other parameter until it fits the desired parameter
    if(width_scalar < height_scalar) {
        //resize to width then extend to height
        
        offset = (ssize_t)(height/2.0 - width_scalar*imgHeight/2.0);

        MagickScaleImage(wand, width, dround(width_scalar*imgHeight));
        MagickExtentImage(wand, width, height, 0, 0);
        MagickRollImage(wand, 0, offset);
    } else {
        //resize to height then extend to width

        offset = (ssize_t)(width/2.0 - height_scalar*imgWidth/2.0);

        MagickScaleImage(wand, dround(height_scalar*imgWidth), height);
        MagickExtentImage(wand, width, height, 0, 0);
        MagickRollImage(wand, offset, 0);
    } 

    // then we store the brightness values in the pixels array using pixel iterators
    PixelIterator *iterator;

    PixelWand **pixel;
    size_t num_pixel;
    
    iterator = NewPixelIterator(wand);
    if(iterator == (PixelIterator *) NULL) {
        ThrowWandException(wand);
    }

    // the loops below basically iterate over all the pixels
    pixel = PixelGetNextIteratorRow(iterator, &num_pixel);
    for(int i = 0; pixel != (PixelWand **) NULL; i++) {
        for(int j = 0 ; j < num_pixel ; j++) {
            // since the image is greyscale the RGB values for the image are equal
            // I arbitrarily chose to get the blue value.
            pixels[i][j] = PixelGetBlue(pixel[j]);
        }

        pixel = PixelGetNextIteratorRow(iterator, &num_pixel);   
    }

    // then free all the allocated memory
    wand = DestroyMagickWand(wand);
    bg = DestroyPixelWand(bg);
    iterator = DestroyPixelIterator(iterator);

    MagickWandTerminus();
}
