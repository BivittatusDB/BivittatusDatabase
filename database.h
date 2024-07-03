#include "base.h"

//CRUD OPERATIONS FOR DATABASE
// C: Create
void CreateDataset(const char* basename, const char* setname, const char* data){
    // Write table header
    SetHeader node;
    snprintf(node.tablename, sizeof(node.tablename), "%s", setname);
    node.datasize = strlen(data);

    // Make sure the dataset is not already created
    assert(CheckDataSet(basename, setname)!=True);

    // Write header and data to the file
    FILE* database = open_file(basename, "ab");
    fwrite(&node, sizeof(node), 1, database);
    fwrite(data, 1, node.datasize, database);
    fclose(database);
}

// R: Read
char* ReadData(FILE* database, int datasize){
    // read data contents of the dataset after set header
    char* buffer = (char*)calloc(datasize+1, sizeof(char));
    fread(buffer,1,datasize, database);
    return buffer;
}

char* GetData(const char* basename, const char* setname){
    //find header and then read data after it
    assert(CheckDataSet(basename, setname)!=True); //I think this should be == and not !=?
    FILE* database = open_file(basename, "rb");
    SetHeader node = LoadSet(database);
    while (strcmp(node.tablename, setname)!=0){
        fseek(database, node.datasize, SEEK_CUR);
        node=LoadSet(database);
    }
    char* buffer = ReadData(database, node.datasize);
    return buffer;
}

// D: Delete (Being written before update for simplicity
void DeleteSet(const char* basename, const char* setname){
    //Find the placement of the requested node, mode all nodes after it up, and truncate the file
    assert(CheckDataSet(basename, setname)!=True);
    FILE* database=open_file(basename, "rb+");
    SetHeader node = LoadSet(database); 
    
    //find set placement
    while (strcmp(node.tablename, setname) != 0){
        fseek(database, node.datasize, SEEK_CUR);
        node = LoadSet(database);
    }
    fseek(database, node.datasize, SEEK_CUR);
    int offset = HEADERSIZE+node.datasize; //offset to move back and forth and overwrite data
    
    do {
        //do while loop ensures the last set it still moved even if it's size is shorter than offset
        node=LoadSet(database);
        char* data=ReadData(database, node.datasize);
        int extra = HEADERSIZE + node.datasize;
        fseek(database, -1*(offset+extra), SEEK_CUR);
        fwrite(&node, sizeof(node), 1, database);
        fwrite(data, 1, node.datasize, database);
        fseek(database, offset, SEEK_CUR);
    } while (DistToEnd(database) > offset);
    
    //truncate the extra data
    fseek(database, -offset, SEEK_CUR);
    ftruncate(fileno(database), ftell(database));
    fclose(database);
}

// U: Update
void UpdateData(const char* basename, const char* setname, const char* data){
    // Update by deleting the old and replacing it with something new... I know stupid... but it works

    DeleteSet(basename, setname);
    CreateDataset(basename, setname, data);
}