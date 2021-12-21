# blind
an ncurses based image viewer for the terminal which shows the image using braille characters

**this is still a prototype**

Usage
-----
At this time, compilation is not yet finished.

To compile you run `make` on the source directory (which requires imagemagick and the magickwand api) then move the executable (`blind`) to your `$PATH`,
but that may lead to errors.

To view `image.png` simply run
```bash
blind /path/to/image.png
```

TODO list:
---------
* implement command line options
* more robust error handling
* make compilation easier
* add color support
