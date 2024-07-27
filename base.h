// Include packages
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h> // for mkdir on Windows
#else
#include <sys/types.h> // for mkdir on Unix-like
#endif

// Code Based Constants
#define MAXFILENAME _MAX_FNAME //should be about 256 for most systems
#define TESTNAME "test.db" //use for testing this code
#define PATHSIZE ((MAXFILENAME*2)+1)

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
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void makedir(const char* dir_name){
    struct stat st = {0};

    if (stat(dir_name, &st) == -1) {
        // The directory does not exist, so try to create it
        if (mkdir(dir_name) != 0) { 
            perror("Couldn't make directory");
            exit(EXIT_FAILURE);
        }
    }
}

char* catpath(const char* subdir, const char* file){
    static char fullpath[PATHSIZE];
    if (sprintf(fullpath, "%s/%s", subdir, file) < 0) {
        fprintf(stderr, "Error concatenating path and file name.\n");
        exit(EXIT_FAILURE);
    }
    return fullpath;
}

int CheckDataSet(const char* tablename){
    struct stat buffer;
    int result = stat(tablename, &buffer);
    if (result == 0){
        return 0; // Success
    } else {
        switch(errno) {
            case ENOENT:
                return 1; // File does not exist
            case EACCES:
                return 2; // Permission denied.
            default:
                return -1; // Unknown error
        }
    }
}

