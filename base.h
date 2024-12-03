#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include "bdb_encrypt/FileEncrypter.h"

#define MAXFILENAME 256 // should be about 256 for most systems
#define TESTNAME "test.db" // use for testing this code
#define PATHSIZE ((MAXFILENAME*2)+1)

// Database Struct
typedef struct {
    char tablename[MAXFILENAME];
    long datasize;
} SetHeader;

#define HEADERSIZE sizeof(SetHeader)

// Other useful functions
FILE* open_file(PATH dir, PATH filename, const char* mode) {
    //decrypt the file
    if (mode != (char*)"w" && mode !=(char*)"wb"){
        KEY privkey=load_privkey(dir);
        if (decrypt_file(dir, filename, privkey) !=0){
            fprintf(stderr, "Error decrypting the file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    //open the file
    PATH filepath = catpath(catpath(dir, "/"), filename);
    FILE* file = fopen(filepath, mode);
    if (!file) {
        fprintf(stderr, "Error opening the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

int close_file(PATH dir, PATH filename, FILE *file){
    fclose(file);
    KEY privkey = load_privkey(dir);
    if (encrypt_file(catpath(dir, "/"), filename, privkey)){
        fprintf(stderr, "Error encrypting the file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return 0;
}

bool CheckDataSet(const char* tablename) {
    FILE* file = fopen(tablename, "r");
    if (file) {
        fclose(file);
        return true; // Success
    } else {
        return false;
    }
}

size_t str_len(const char *str) {
    const char *start = str;           // Save start address
    while (*str != '\0') {
        str++;                         // loop through characters until teminator is found
    }
    return str - start;                // return the difference in memory address for length
}