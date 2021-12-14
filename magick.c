#include <stdlib.h>

#include <MagickWand/MagickWand.h>

#include "magick.h"
#include "helpers.h"

#define ThrowWandException(wand) { \
    char *description; \
    ExceptionType severity; \
    \
    description = MagickGetException(wand, &severity); \
    (void) fprintf(stderr, "%s %s %lu %s \n", GetMagickModule(), description); \
    description = (char *) MagickRelinquishMemory(description); \
    exit(-1); \
}

void GetPixelValues(FILE* img, double **pixels, const size_t width, const size_t height) {
    MagickBooleanType status;
    MagickWand* wand;
    PixelWand* bg;

    MagickWandGenesis();
    wand = NewMagickWand();

    bg = NewPixelWand();
    PixelSetColor(bg, "black");

    MagickSetBackgroundColor(wand, bg);

    status = MagickReadImageFile(wand, img);
    if(status == MagickFalse) {
        ThrowWandException(wand);
    }

    MagickSetImageColorspace(wand, GRAYColorspace);

    size_t imgWidth, imgHeight;
    imgWidth = MagickGetImageWidth(wand);
    imgHeight = MagickGetImageHeight(wand);

    double width_scalar, height_scalar;
    ssize_t offset = 0;
    
    width_scalar = (double)width/imgWidth;
    height_scalar = (double)height/imgHeight;

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
}
