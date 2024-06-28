LIBS = -lraylib
FILES = src/main.c

all:
	gcc $(LIBS) $(FILES) -o build/main;
	build/main
