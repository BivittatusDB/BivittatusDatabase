# gcc -c -Wall -Werror -fpic database.c
# gcc -shared -o lib_bdb.so database.o

# Variables
CC = gcc
CFLAGS = -c -Wall -Werror -fpic
SHARED = -shared -o
WIN_TARGET = lib_bdb_win32.so
ELF_TARGET = lib_bdb_elf.so
OBJ = database.o
SRC = database.c
HEADER = database.h
TESTFLAG = -f

# Targets
all: rename compile link

win: winrename compile win_link

rename:
	@if [ $(TESTFLAG) $(HEADER) ]; then mv $(HEADER) $(SRC); fi

winrename:
	@if not exist $(SRC) if exist $(HEADER) ren $(HEADER) $(SRC)

compile: $(SRC)
	$(CC) $(CFLAGS) $(SRC)

link: $(OBJ)
	$(CC) $(SHARED) $(ELF_TARGET) $(OBJ)

win_link: $(OBJ)
	$(CC) $(SHARED) $(WIN_TARGET) $(OBJ)

clean:
	rm -f $(OBJ)

winclean:
	-del /f $(OBJ)
