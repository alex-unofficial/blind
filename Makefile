CC = gcc

LIBS = ncursesw MagickWand

CFLAGS = $(shell pkg-config --cflags $(LIBS))-g
LDFLAGS = $(shell pkg-config --libs $(LIBS))

exec = blind

headers = $(wildcard *.h) 
obj = $(headers:.h=.o)

.PHONY: all clean

all: $(exec)

$(exec): blind.o $(obj)
	$(CC) $(CFLAGS) $^ $(LDFLAGS)-o $@

blind.o: blind.c $(headers)
	$(CC) $(CFLAGS) -c $<

$(obj): %.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(wildcard *.o)
	rm -f $(exec)

