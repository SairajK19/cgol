LIBS = -lraylib
FILES = src/main.c
C_FLAGS = -ggdb

all:
	gcc $(LIBS) $(C_FLAGS) $(FILES) -o build/cgol;
