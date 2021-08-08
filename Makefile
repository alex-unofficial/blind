braille: braille.c
	gcc `pkg-config --cflags ncursesw` braille.c `pkg-config --libs ncursesw` -lm -o exec

clean:
	rm exec
