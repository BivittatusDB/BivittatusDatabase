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
FILE* open_file(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void makedir(const char* dir_name) {
    struct stat st;

    // Check if the directory exists
    if (stat(dir_name, &st) == 0) {
        // Directory exists
        printf("Directory already exists: %s\n", dir_name);
    } else if (errno == ENOENT) {
        // Directory does not exist, try to create it
        #ifdef _WIN32
            if (_mkdir(dir_name) != 0) {
                perror("Could not create directory");
                exit(EXIT_FAILURE);
            }
        #else
            if (mkdir(dir_name, 0750) != 0) {
                perror("Could not create directory");
                exit(EXIT_FAILURE);
            }
        #endif
        printf("Directory created: %s\n", dir_name);
    } else {
        // Some other error occurred
        perror("Error checking directory");
        exit(EXIT_FAILURE);
    }
}

char* catpath(const char* subdir, const char* file) {
    static char fullpath[PATHSIZE];
    if (snprintf(fullpath, PATHSIZE, "%s/%s", subdir, file) >= PATHSIZE) {
        fprintf(stderr, "Error concatenating path and filename.\n");
        exit(EXIT_FAILURE);
    }
    return fullpath;
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
