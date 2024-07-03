//After any rewriting, this code should still be able to run, as it tests all ma
// Make sure there is no file "Hello" left from previous attempts or there will be assertion errors
#include "database.h"

int main(){
    CreateDataset("Hello", "Table1", "testing");
    CreateDataset("Hello", "table2", "testing again");
    CreateDataset("Hello", "table3.00", "Hello World longer this time");
    UpdateData("Hello", "table2", "testing updated");
    char* buffer = GetData("Hello", "table2");
    printf("%s", buffer);
    return False;
}