# gcc -c -Wall -Werror -fpic database.c
# gcc -shared -o lib_bdb.so database.o

# Variables
CC = gcc
CFLAGS = -c -Wall -Werror -fpic
SHARED = -shared -o
TARGET = lib_bdb.so
OBJ = database.o
SRC = database.c
HEADER = database.h
TESTFLAG = -f

# Targets
all: rename compile link

win: winrename compile link

rename:
	@if [ $(TESTFLAG) $(HEADER) ]; then mv $(HEADER) $(SRC); fi

winrename:
	@if not exist $(SRC) if exist $(HEADER) ren $(HEADER) $(SRC)

compile: $(SRC)
	$(CC) $(CFLAGS) $(SRC)

link: $(OBJ)
	$(CC) $(SHARED) $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)

winclean:
	-del /f $(OBJ)
