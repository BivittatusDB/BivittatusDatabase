//After any rewriting, this code should still be able to run, as it tests all ma
// Make sure there is no directory "Hello" left from previous attempts or there will be assertion errors (FIXED?)
#include "database.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    CreateDatabase("Hello");

    CreateTable("Hello", TESTNAME, "Hello World");
    int check = CheckDataSet(catpath("Hello", TESTNAME));
    printf("Test table 1 exists: %d\n", check);

    CreateTable("Hello", "hello_world", "check");
    check = CheckDataSet(catpath("Hello", "hello_world"));
    printf("Test table 2 exists: %d\n", check);

    AddMetaData("Hello", "hello_world", "working...");
    char* data = ReadTable("Hello", "hello_world", 0); //False
    char* metadata = ReadTable("Hello", "hello_world", 1); //True
    printf("%s: %s\n", data, metadata);

    DeleteTable("Hello", TESTNAME);
    check = CheckDataSet(catpath("Hello", TESTNAME));
    printf("Error in file deletion: %d\n", check);

    UpdateTable("Hello", "hello_world", "new check");
    data = ReadTable("Hello", "hello_world", 0); //False
    metadata = ReadTable("Hello", "hello_world", 1); //True
    printf("%s: %s\n", data, metadata);

    //Free memory
    free(data);
    free(metadata);

    return 0;
}