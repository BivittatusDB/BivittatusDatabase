#include "base.h"

//CRUD OPERATIONS FOR DATABASE
// C: Create
void CreateDatabase(const char* databasename){
    makedir(databasename);
}

void CreateTable(const char* databasename, const char* tablename, const char* data){
    if (CheckDataSet(catpath(databasename, tablename))==False){
        SetHeader header;
        snprintf(header.tablename, sizeof(header.tablename), "%s", tablename);
        header.datasize=strlen(data);

        FILE* file = open_file(catpath(databasename, tablename), "wb");
        fwrite(&header, HEADERSIZE, 1, file);
        fwrite(data, 1, header.datasize, file);
        fclose(file);
    }
}

void AddMetaData(const char* databasename, const char* tablename, const char* metadata){
    if (CheckDataSet(catpath(databasename, tablename))==True){
        SetHeader MetaHeader;
        snprintf(MetaHeader.tablename, sizeof(MetaHeader.tablename), "meta_%s", tablename);
        MetaHeader.datasize=strlen(metadata);

        FILE* file = open_file(catpath(databasename, tablename), "ab");
        fwrite(&MetaHeader, HEADERSIZE, 1, file);
        fwrite(metadata, 1, MetaHeader.datasize, file);
        fclose(file);
    }
}

// R: Read
char* ReadTable(const char* databasename, const char* tablename, bool Metadata){
    if (CheckDataSet(catpath(databasename, tablename))==True){
        FILE* file = open_file(catpath(databasename, tablename), "rb");
        SetHeader Header;
        if (fread(&Header, HEADERSIZE, 1, file) == True){
            char* buffer = (char*)calloc(Header.datasize+1, sizeof(char));
            fread(buffer, 1, Header.datasize, file);
            if (Metadata==False){
                fclose(file);
                return buffer;
            }
        } else {exit(1);}
        SetHeader MetaHeader;
        if (fread(&MetaHeader, HEADERSIZE, 1, file)==True){
            char* buffer = (char*)calloc(MetaHeader.datasize+1, sizeof(char));
            fread(buffer, 1, MetaHeader.datasize, file);
            fclose(file);
            return buffer;
        } else {exit(1);}
    }
    //will never reach here... just so the compiler shuts up
    return "...";
}

// D: Delete (Being written before update for simplicity)
void DeleteTable(const char* databasename, const char* tablename){
    char* path = catpath(databasename, tablename);
    if (CheckDataSet(path) == True){
        if (remove(path) != 0){
            perror("Couldn't delete table");
            exit(1);
        }
    }
}

// U: Update
void UpdateTable(const char* databasename, const char* tablename, const char* data){
    char* metadata=ReadTable(databasename, tablename, True);
    DeleteTable(databasename, tablename);
    CreateTable(databasename, tablename, data);
    AddMetaData(databasename, tablename, metadata);
}