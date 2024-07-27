#include "base.h"
#include <string.h>
// 1 = true / 0 = False
// CRUD OPERATIONS FOR DATABASE
// C: Create
void CreateDatabase(const char* databasename){
      // Create a directory with the name of the database
    makedir(databasename);
}

void CreateTable(const char* databasename, const char* tablename, const char* data){
    // If the table does not exist, create a new table in the database.
    if (CheckDataSet(catpath(databasename, tablename))==0){
        // Prepare the table header
        SetHeader header;
        snprintf(header.tablename, sizeof(header.tablename), "%s", tablename);
        header.datasize=strlen(data);

        // Open the table file and write the header and the data
        FILE* file = open_file(catpath(databasename, tablename), "wb");
        fwrite(&header, HEADERSIZE, 1, file);
        fwrite(data, 1, header.datasize, file);
        fclose(file);
    }
}

void AddMetaData(const char* databasename, const char* tablename, const char* metadata){
    // If the table exists, add metadata to the table
    if (CheckDataSet(catpath(databasename, tablename))==1){
        // Prepare the metadata header
        SetHeader MetaHeader;
        snprintf(MetaHeader.tablename, sizeof(MetaHeader.tablename), "meta_%s", tablename);
        MetaHeader.datasize=strlen(metadata);

        // Open the table file and write the metadata header and the metadata
        FILE* file = open_file(catpath(databasename, tablename), "ab");
        fwrite(&MetaHeader, HEADERSIZE, 1, file);
        fwrite(metadata, 1, MetaHeader.datasize, file);
        fclose(file);
    }
}

// R: Read
char* ReadTable(const char* databasename, const char* tablename, int Metadata){
    // If the table exists, reads the data or metadata from the table.
    if (CheckDataSet(catpath(databasename, tablename))==1){
        FILE* file = open_file(catpath(databasename, tablename), "rb");
        SetHeader Header;
        if (fread(&Header, HEADERSIZE, 1, file) == 1){
            char* buffer = (char*)calloc(Header.datasize+1, sizeof(char));
            fread(buffer, 1, Header.datasize, file);
            if (Metadata==0){
                fclose(file);
                return buffer;
            }
        } else {exit(EXIT_FAILURE);}
        SetHeader MetaHeader;
        if (fread(&MetaHeader, HEADERSIZE, 1, file)==1){
            char* buffer = (char*)calloc(MetaHeader.datasize+1, sizeof(char));
            fread(buffer, 1, MetaHeader.datasize, file);
            fclose(file);
            return buffer;
        } else {exit(EXIT_FAILURE);}
    } else {exit(EXIT_FAILURE);}
}

// D: Delete (Being written before update for simplicity)
void DeleteTable(const char* databasename, const char* tablename){
    // If the table exists, delete the table
    char* path = catpath(databasename, tablename);
    if (CheckDataSet(path) == 1){
        if (remove(path) != 0){
            perror("No se pudo borrar la tabla");
            exit(EXIT_FAILURE);
        }
    }
}

// U: Update
void UpdateTable(const char* databasename, const char* tablename, const char* data){
        /*Reads the metadata from the table, deletes the table,
        creates a new table with the new data and adds the metadata*/ 
    char* metadata=ReadTable(databasename, tablename, 1);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
}
<<<<<<< Updated upstream

void UpdateMetaTable(const char* databasename, const char* tablename, const char* metadata){
    char* data=ReadTable(databasename, tablename, False);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
}
=======
>>>>>>> Stashed changes
