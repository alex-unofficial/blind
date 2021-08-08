braille: braille.c
	gcc `pkg-config --cflags ncursesw` braille.c `pkg-config --libs ncursesw` -o exec

clean:
	rm exec
