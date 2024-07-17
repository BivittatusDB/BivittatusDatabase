// Include packages
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>

// Code Based Constants
#define MAXFILENAME _MAX_FNAME //should be about 256 for most systems
#define TESTNAME "test.db" //use for testing this code
#define PATHSIZE ((MAXFILENAME*2)+1)
char fullpath[PATHSIZE];

#define bool int // Bools since they aren't built-in
#define False 0
#define True 1

// Database Struct
typedef struct {
    char tablename[MAXFILENAME];
    long datasize;
} SetHeader;

#define HEADERSIZE sizeof(SetHeader)

// Other useful functions
FILE* open_file(const char* filename, const char* mode){
    //open file with error handling
    FILE* file = fopen(filename, mode);
    if (!file) {
        perror("Couldn't open file");
        exit(5);
    }
    return file;
}

void makedir(const char* dir_name){
    if (mkdir(dir_name) != False) { 
        perror("Couldn't make directory");
        exit(1);
    }
}

char* catpath(const char* subdir, const char* file){
    sprintf(fullpath, "%s/%s", subdir, file);
    return fullpath;
}

bool CheckDataSet(const char* tablename){
    struct stat buffer;
    if (stat(tablename, &buffer)==0){
        return True;
    } else {
        return False;
    }
}