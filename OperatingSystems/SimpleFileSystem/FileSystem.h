//
// Created by burak on 08-Jun-21.
//

#ifndef E2235646_EE442_HW3_FILESYSTEM_H
#define E2235646_EE442_HW3_FILESYSTEM_H

#endif //E2235646_EE442_HW3_FILESYSTEM_H

#define CHUNK_SIZE 512
#include <stdio.h>
#include <string.h>
typedef struct
{
    char data[CHUNK_SIZE];
}DataEntry;
typedef struct {
    DataEntry list[4096];
}Data;
typedef struct
{
    unsigned int entry;
}FATEntry;
typedef struct
{
    FATEntry list[4096];
}FAT;
typedef struct
{
    char fileName[248];
    unsigned int size;
    unsigned int firstBlock;
} FileListEntry;

typedef struct
{
    FileListEntry list[128];
}FileList;

DataEntry buffer;
FILE* ptrFile;
unsigned int convertToLittleEndian(unsigned int bigEndianNum);
unsigned int convertToBigEndian(unsigned int littleEndianNum);
void Format(const char* disk);
void Write(const char* disk, const char* srcPath,const char* destFileName);
void Read(const char* disk, const char* srcFileName,const char* destPath);
void Delete(const char* disk, const char* filename);
void List(const char* disk);
void Defragment(const char* disk);
