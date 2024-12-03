# gcc -c -Wall -Werror -fpic database.c
# gcc -shared -o lib_bdb.so database.o

# Variables
CC = gcc
CFLAGS = -c -Wall -Werror -fpic
ENC_FLAGS = -lcrypto -lssl3 -loqs
SHARED = -shared -o
WIN32_TARGET = lib_bdb_win32.dll
WIN32_TARGET_EXP = lib_bdb_win32_exp.dll
WIN64_TARGET = lib_bdb_win64.dll
WIN64_TARGET_EXP = lib_bdb_win64_exp.dll
ELF_TARGET = lib_bdb_elf.so
ELF_TARGET_EXP = lib_bdb_elf_exp.so
OBJ = database.o
SRC = database.c
HEADER = database.h
TESTFLAG = -f
EXPERIMENT = -DBDB_USE_EXPERIMENTAL

#UNIX
rename:
	@if [ $(TESTFLAG) $(HEADER) ]; then mv $(HEADER) $(SRC); fi

compile: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) $(SRC)

compile-E: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) $(EXPERIMENT) $(SRC)

link: $(OBJ)
	$(CC) $(SHARED) $(ELF_TARGET) $(OBJ)

link-E: $(OBJ)
	$(CC) $(SHARED) $(ELF_TARGET_EXP) $(OBJ)

-UNIX: rename compile link 
-UNIX-E: rename compile-E link-E

clean:
	rm -f $(OBJ)

#WIN
winrename:
	@if not exist $(SRC) if exist $(HEADER) ren $(HEADER) $(SRC)

winclean:
	-del /f $(OBJ)

#WIN32
compile32: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) -m32 $(SRC)

compile32-E: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) -m32 $(EXPERIMENT) $(SRC)

win32_link: $(OBJ)
	$(CC) $(SHARED) $(WIN32_TARGET) $(OBJ)

win32_link-E: $(OBJ)
	$(CC) $(SHARED) $(WIN32_TARGET_EXP) $(OBJ)

-32: winrename compile32 win32_link
-32-E: winrename compile32-E win32_link-E

#WIN64
compile64: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) -m64 $(SRC)

compile64-E: $(SRC)
	$(CC) $(CFLAGS) $(ENC_FLAGS) -m64 $(EXPERIMENT) $(SRC)

win64_link: $(OBJ)
	$(CC) $(SHARED) $(WIN64_TARGET) $(OBJ)

win64_link-E: $(OBJ)
	$(CC) $(SHARED) $(WIN64_TARGET_EXP) $(OBJ)

-64: winrename compile64 win64_link
-64-E: winrename compile64-E win64_link-E

