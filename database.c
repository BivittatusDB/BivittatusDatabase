#include "base.h"
#include <string.h>
#include <stdbool.h>

// CRUD OPERATIONS FOR DATABASE
// C: Create
void CreateDatabase(const char* databasename){
    // Create a directory with the name of the database
    makedir(databasename);
}

void CreateTable(const char* databasename, const char* tablename, const char* data){
    // If the table does not exist, create a new table in the database.
    if (CheckDataSet(catpath(databasename, tablename))==false){
        // Prepare the table header
        SetHeader header;
        snprintf(header.tablename, sizeof(header.tablename), "%s", tablename);
        header.datasize=strnlen(data, sizeof(header.datasize));

        // Open the table file and write the header and the data
        FILE* file = open_file(catpath(databasename, tablename), "wb");
        fwrite(&header, HEADERSIZE, 1, file);
        fwrite(data, 1, header.datasize, file);
        fclose(file);
    }
}

void AddMetaData(const char* databasename, const char* tablename, const char* metadata){
    // If the table exists, add metadata to the table
    if (CheckDataSet(catpath(databasename, tablename))==true){
        // Prepare the metadata header
        SetHeader MetaHeader;
        snprintf(MetaHeader.tablename, sizeof(MetaHeader.tablename), "meta_%s", tablename);
        MetaHeader.datasize=strnlen(metadata, sizeof(MetaHeader.datasize));

        // Open the table file and write the metadata header and the metadata
        FILE* file = open_file(catpath(databasename, tablename), "ab");
        fwrite(&MetaHeader, HEADERSIZE, 1, file);
        fwrite(metadata, 1, MetaHeader.datasize, file);
        fclose(file);
    }
}

// R: Read
char* ReadTable(const char* databasename, const char* tablename, bool Metadata){
    // If the table exists, reads the data or metadata from the table.
    if (CheckDataSet(catpath(databasename, tablename))==true){
        FILE* file = open_file(catpath(databasename, tablename), "rb");
        if (file == NULL) {
            perror("Error opening file");
            return NULL;
        }

        SetHeader Header;
        if (fread(&Header, HEADERSIZE, 1, file) != 1) {
            fclose(file);
            perror("Error reading header");
            return NULL;
        }

        if (Header.datasize <= 0) {
            fclose(file);
            perror("Invalid data size");
            return NULL;
        }

        char* buffer = (char*)calloc(Header.datasize+1, sizeof(char));
        if (buffer == NULL) {
            fclose(file);
            perror("Memory allocation failed");
            return NULL;
        }

        if (fread(buffer, 1, Header.datasize, file) != Header.datasize) {
            free(buffer);
            fclose(file);
            perror("Error reading data");
            return NULL;
        }

        if (Metadata == false) {
            fclose(file);
            return buffer;
        } else {
            free(buffer);
        }

        SetHeader MetaHeader;
        if (fread(&MetaHeader, HEADERSIZE, 1, file) != 1) {
            fclose(file);
            perror("Error reading metadata header");
            return NULL;
        }

        if (MetaHeader.datasize <= 0) {
            fclose(file);
            perror("Invalid metadata size");
            return NULL;
        }

        buffer = (char*)calloc(MetaHeader.datasize+1, sizeof(char));
        if (buffer == NULL) {
            fclose(file);
            perror("Memory allocation failed");
            return NULL;
        }

        if (fread(buffer, 1, MetaHeader.datasize, file) != MetaHeader.datasize) {
            free(buffer);
            fclose(file);
            perror("Error reading metadata");
            return NULL;
        }

        fclose(file);
        return buffer;
    } else {
        perror("Table does not exist");
        return NULL;
    }
}

// D: Delete (Being written before update for simplicity)
void DeleteTable(const char* databasename, const char* tablename){
    // If the table exists, delete the table
    const char* path = catpath(databasename, tablename);
    if (CheckDataSet(path) && remove(path) != 0){
        perror("Could not delete the table");
        exit(EXIT_FAILURE);
    }
}

// U: Update
void UpdateTable(const char* databasename, const char* tablename, const char* data){
    /*Reads the metadata from the table, deletes the table,
    creates a new table with the new data and adds the metadata*/ 
    char* metadata = ReadTable(databasename, tablename, true);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
    free(metadata); // Free the memory allocated to metadata
}

void UpdateMetaTable(const char* databasename, const char* tablename, const char* metadata){
    char* data = ReadTable(databasename, tablename, false);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
    free(data); // Release the memory allocated to data
}
