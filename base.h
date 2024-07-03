// Include packages
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32 // Define Windows behavior:
    #include <io.h>
    #define ftruncate _chsize
#else // Define Unix Behavior: 
    #include <unistd.h>
#endif

// Code Based Constants
#define MAXFILENAME _MAX_FNAME
#define TESTNAME "test.db" //use for testing this code

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

int DistToEnd(FILE* file) {
    //get distance to end of the file
    int current = ftell(file);
    fseek(file, 0, SEEK_END);
    int end=ftell(file);
    fseek(file, current, SEEK_SET);
    return (end-current);
}

SetHeader LoadSet(FILE* file){
    //load a set from a file at current position
    SetHeader node;
    if (feof(file)){
        //if end of file, return empty header
        node.datasize=False;
        return node;
    }
    if (fread(&node, sizeof(SetHeader), 1, file) == True){
        // If another header is found, return it
        return node;
    }
    //if fread fails, return empty header
    node.datasize=False;
    return node;
}

bool CheckDataSet(const char* basename, const char* setname){
    FILE* database = open_file(basename, "ab");
    SetHeader node = LoadSet(database); //get a header
    while (node.datasize != 0) { //while no error and not end of file
        if (strcmp(node.tablename, setname)==0){
            return True; //return true if the set names match (e.g. set in database)
        }
        node=LoadSet(database); // try again if not
    }
    return False; //return false if error or end of file
}