#include <stdlib.h>

#include <MagickWand/MagickWand.h>

#include "magick.h"

void ThrowWandException(MagickWand* wand) {
    char *description;
    ExceptionType severity;
    
    description = MagickGetException(wand, &severity);
    (void) fprintf(stderr, "%s %s %lu %s \n", GetMagickModule(), description);
    description = (char *) MagickRelinquishMemory(description);
    exit(-1);
}
