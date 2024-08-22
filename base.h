#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

#ifdef _WIN32
#include <direct.h> // for mkdir on Windows
#else
#include <sys/types.h> // for mkdir on Unix-like systems
#endif

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
FILE* open_file(const char* filename, const char* mode){
    // open file with error handling
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
        #ifdef _WIN32
            if (_mkdir(dir_name) != 0) {
                if (errno != EEXIST) {
                    perror("Couldn't make directory");
                    exit(EXIT_FAILURE);
                }
            }
        #else
            if (mkdir(dir_name, 0755) != 0) {
                if (errno != EEXIST) {
                    perror("Couldn't make directory");
                    exit(EXIT_FAILURE);
                }
            }
        #endif
    } else if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Path exists but is not a directory: %s\n", dir_name);
        exit(EXIT_FAILURE);
    }
}

char* catpath(const char* subdir, const char* file){
    static char fullpath[PATHSIZE];
    if (snprintf(fullpath, PATHSIZE, "%s/%s", subdir, file) >= PATHSIZE) {
        fprintf(stderr, "Error concatenating path and file name.\n");
        exit(EXIT_FAILURE);
    }
    return fullpath;
}

bool CheckDataSet(const char* tablename){
    FILE* file = fopen(tablename, "r");
    if (file) {
        fclose(file);
        return true; // Success
    } else {
        return false;
    }
}
