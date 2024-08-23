//After any rewriting, this code should still be able to run, as it tests all ma
// Make sure there is no directory "Hello" left from previous attempts or there will be assertion errors (FIXED?)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "database.h"

int main() {
    CreateDatabase("Hello");

    CreateTable("Hello", TESTNAME, "Hello World");
    bool check = CheckDataSet(catpath("Hello", TESTNAME));
    printf("Test table 1 exists: %s\n", check ? "True" : "False");

    CreateTable("Hello", "hello_world", "check");
    check = CheckDataSet(catpath("Hello", "hello_world"));
    printf("Test table 2 exists: %s\n", check ? "True" : "False");

    AddMetaData("Hello", "hello_world", "working...");
    char* data = ReadTable("Hello", "hello_world", false);
    char* metadata = ReadTable("Hello", "hello_world", true);
    printf("%s: %s\n", data, metadata);

    DeleteTable("Hello", TESTNAME);
    check = CheckDataSet(catpath("Hello", TESTNAME));
    printf("Result of the elimination from the table: %s\n", check ? "False" : "Success");

    UpdateTable("Hello", "hello_world", "new check");
    data = ReadTable("Hello", "hello_world", false);
    metadata = ReadTable("Hello", "hello_world", true);
    printf("%s: %s\n", data, metadata);

    //Free memory
    free(data);
    free(metadata);

    return 0;
}