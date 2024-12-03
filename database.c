#include <string.h>
#include <stdbool.h>
#include "base.h"

// CRUD OPERATIONS FOR DATABASE
// C: Create
void CreateDatabase(PATH databasename){
    // Create a directory with the name of the database, and generate keys (PUB, PRIV, and SYM)
    makedir(databasename);
    gen_keys(databasename);
    KEY pub = load_pubkey(databasename);
    ensure_shared_key(pub, databasename);
}

void CreateTable(PATH databasename, PATH tablename, const char* data){
    // If the table does not exist, create a new table in the database.
    if (CheckDataSet(catpath(databasename, tablename))==false){
        // Prepare the table header
        SetHeader header;
        snprintf(header.tablename, sizeof(header.tablename), "%s", tablename);
        header.datasize=str_len(data);
        // Open the table file and write the header and the data
        FILE* file = open_file(databasename, tablename, "wb");
        if (!file) exit(EXIT_FAILURE);
        fwrite(&header, HEADERSIZE, 1, file);
        fwrite(data, 1, header.datasize, file);
        close_file(databasename,tablename, file);
    }
}

void AddMetaData(PATH databasename, PATH tablename, const char* metadata){
    // If the table exists, add metadata to the table
    if (CheckDataSet(catpath(databasename, tablename))==true){
        // Prepare the metadata header
        SetHeader MetaHeader;
        snprintf(MetaHeader.tablename, sizeof(MetaHeader.tablename), "meta_%s", tablename);
        MetaHeader.datasize=str_len(metadata);

        // Open the table file and write the metadata header and the metadata
        FILE* file = open_file(databasename, tablename, "ab");
        fwrite(&MetaHeader, HEADERSIZE, 1, file);
        fwrite(metadata, 1, MetaHeader.datasize, file);
        close_file(databasename, tablename, file);
    }
}

// R: Read
char* ReadTable(PATH databasename, PATH tablename, bool Metadata){
    // If the table exists, reads the data or metadata from the table.
    if (CheckDataSet(catpath(databasename, tablename))==true){
        FILE* file = open_file(databasename, tablename, "rb");
        if (file == NULL) {
            perror("Error opening file");
            return NULL;
        }

        SetHeader Header;
        if (fread(&Header, HEADERSIZE, 1, file) != 1) {
            close_file(databasename, tablename, file);
            perror("Error reading header");
            return NULL;
        }

        if (Header.datasize <= 0) {
            close_file(databasename, tablename, file);
            perror("Invalid data size");
            return NULL;
        }

        char* buffer = (char*)calloc(Header.datasize+1, sizeof(char));
        if (buffer == NULL) {
            close_file(databasename, tablename, file);
            perror("Memory allocation failed");
            return NULL;
        }

        if (fread(buffer, 1, Header.datasize, file) != Header.datasize) {
            free(buffer);
            close_file(databasename, tablename, file);
            perror("Error reading data");
            return NULL;
        }

        if (Metadata == false) {
            close_file(databasename, tablename, file);
            return buffer;
        } else {
            free(buffer);
        }

        SetHeader MetaHeader;
        if (fread(&MetaHeader, HEADERSIZE, 1, file) != 1) {
            close_file(databasename, tablename, file);
            perror("Error reading metadata header");
            return NULL;
        }

        if (MetaHeader.datasize <= 0) {
            close_file(databasename, tablename, file);
            perror("Invalid metadata size");
            return NULL;
        }

        buffer = (char*)calloc(MetaHeader.datasize+1, sizeof(char));
        if (buffer == NULL) {
            close_file(databasename, tablename, file);
            perror("Memory allocation failed");
            return NULL;
        }

        if (fread(buffer, 1, MetaHeader.datasize, file) != MetaHeader.datasize) {
            free(buffer);
            close_file(databasename, tablename, file);
            perror("Error reading metadata");
            return NULL;
        }

        close_file(databasename, tablename, file);
        return buffer;
    } else {
        perror("Table does not exist");
        return NULL;
    }
}

// D: Delete (Being written before update for simplicity)
void DeleteTable(PATH databasename, PATH tablename){
    // If the table exists, delete the table
    const char* path = catpath(catpath(databasename, "/"), tablename);
    if (CheckDataSet(path) && remove(path) != 0){
        perror("Could not delete the table");
        exit(EXIT_FAILURE);
    }
}

// U: Update
void UpdateTable(PATH databasename, PATH tablename, const char* data){
    /*Reads the metadata from the table, deletes the table,
    creates a new table with the new data and adds the metadata*/ 
    char* metadata = ReadTable(databasename, tablename, true);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
    free(metadata); // Free the memory allocated to metadata
}

void UpdateMetaTable(PATH databasename, PATH tablename, const char* metadata){
    char* data = ReadTable(databasename, tablename, false);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
    free(data); // Release the memory allocated to data
}
