//
// Created by burak on 08-Jun-21.
//

#include "FileSystem.h"
unsigned int convertToLittleEndian(unsigned int bigEndianNum)
{
    unsigned int tempNum = 0;
    tempNum = ((bigEndianNum & 0xFF) << 24) | ((bigEndianNum & 0xFF00) << 8)
            | ((bigEndianNum & 0xFF0000) >> 8) | ((bigEndianNum & 0xFF000000) >> 24);

    return  tempNum;
}
unsigned int convertToBigEndian(unsigned int littleEndianNum)
{
    unsigned int tempNum = 0;
    tempNum = ((littleEndianNum & 0xFF) << 24) | ((littleEndianNum & 0xFF00) << 8)
            | ((littleEndianNum & 0xFF0000) >> 8) | ((littleEndianNum & 0xFF000000) >> 24);

    return  tempNum;
}
// Format the contents of the disk.
void Format(const char* disk)
{
    FAT emptyFAT;
    memset(&emptyFAT,0,sizeof(FAT));
    // first entry must always be 0xFFFFFFFF
    emptyFAT.list[0].entry = 0xFFFFFFFF;
    FileListEntry emptyFileList;
    emptyFileList.size = 0;
    emptyFileList.firstBlock =0;
    memset(emptyFileList.fileName,0,248);
    emptyFileList.fileName[0] = '\0';
    DataEntry emptyDataEntry;
    memset(&emptyDataEntry, 0, CHUNK_SIZE);

    ptrFile = fopen(disk,"w");
    fwrite(&emptyFAT,sizeof(FAT),1,ptrFile);
    int i=0;
    for(;i<128;i++)
    {
        fwrite(&emptyFileList,sizeof(FileListEntry),1,ptrFile);
    }
    for(i=0; i<4096;i++)
    {
        fwrite(&emptyDataEntry, sizeof(DataEntry), 1, ptrFile);
    }
    fclose(ptrFile);
}

void Write(const char* disk, const char* srcPath,const char* destFileName)
{
    FAT stFAT;
    FileList  stFileList;
    Data stData;

    ptrFile = fopen(disk,"w+");
    fread(&stFAT,sizeof(FAT),1,ptrFile);
    fread(&stFileList,sizeof (FileList),1,ptrFile);
    fread(&stData,sizeof(Data),1,ptrFile);
    int i=1;
    int firstFATBlock = -1;
    for(; i < 4096 ; i++)
    {
        if(stFAT.list[i].entry == 0)
        {
            firstFATBlock = i;
            break;
        }
    }
    if(firstFATBlock == -1)
    {
        printf("[WARNING]:Empty FAT can not be found.\n");
        return;
    }
    int fileListEntryIndex = -1;
    for(i=0; i< 128 ; i++)
    {
        if(stFileList.list[i].size == 0 && stFileList.list[i].firstBlock == 0 )
        {
            fileListEntryIndex = i;
            break;
        }
    }
    if(fileListEntryIndex == -1)
    {
        printf("[WARNING]:No space in file list.\n");
        return;
    }
    // enter the entry to the file list
    i=0;
    while(i < 248)
    {
        stFileList.list[fileListEntryIndex].fileName[i] = destFileName[i];
        if(destFileName[i] == '\0')
            break;
    }
    stFileList.list[fileListEntryIndex].firstBlock = firstFATBlock;
    FILE* sourceReader = fopen(srcPath,"r");
    unsigned int byteCount;
    byteCount =(unsigned int) fread(buffer.data,1,CHUNK_SIZE,sourceReader);
    if(feof(sourceReader))
    {
        memset(stData.list[firstFATBlock].data,0,CHUNK_SIZE);
        stFileList.list[fileListEntryIndex].size = byteCount;
        stFAT.list[firstFATBlock].entry = 0xFFFFFFFF;
        memcpy(stData.list[firstFATBlock].data,buffer.data,byteCount);
    }
    else
    {
        unsigned int currentFATBlock = firstFATBlock;
        unsigned int nextFATBlock = 0xFFFFFFFF;
        int j = 0;
        for(;j<4096 ; j++)
        {
            // find another empty block
            for(i=0; i < 4096 ; i++)
            {
                if(stFAT.list[i].entry == 0)
                {
                    nextFATBlock = i;
                    break;
                }
            }
            if(nextFATBlock == 0xFFFFFFFF)
            {
                printf("[WARNING]: No empty space while writing.\n");
                return;
            }
            memset(buffer.data,0,CHUNK_SIZE);
            byteCount =(unsigned int) fread(buffer.data,1,CHUNK_SIZE,sourceReader);
            if(feof(sourceReader))
            {
                memset(stData.list[currentFATBlock].data,0,CHUNK_SIZE);
                stFileList.list[fileListEntryIndex].size += byteCount;
                stFAT.list[currentFATBlock].entry = 0xFFFFFFFF;
                memcpy(stData.list[currentFATBlock].data,buffer.data,byteCount);
                break;
            }
            else
            {
                memset(stData.list[currentFATBlock].data,0,CHUNK_SIZE);
                stFAT.list[currentFATBlock].entry = convertToBigEndian(nextFATBlock);
                stFileList.list[fileListEntryIndex].size += CHUNK_SIZE;
                memcpy(stData.list[currentFATBlock].data,buffer.data,CHUNK_SIZE);
                currentFATBlock = nextFATBlock;
            }
        }
    }
    fclose(sourceReader);
    // set ptr to beginning of the file.
    fseek(ptrFile,0,SEEK_SET);
    fwrite(&stFAT,sizeof(FAT),1,ptrFile);
    fwrite(&stFileList,sizeof(FileList),1,ptrFile);
    fwrite(&stData,sizeof(Data),1,ptrFile);
    fclose(ptrFile);
}

// Read the contents of a file in the disk to destination path.
void Read(const char* disk, const char* srcFileName,const char* destPath)
{
    FAT stFAT;
    FileList  stFileList;
    Data stData;

    ptrFile = fopen(disk,"r");
    fread(&stFAT,sizeof(FAT),1,ptrFile);
    fread(&stFileList,sizeof (FileList),1,ptrFile);
    fread(&stData,sizeof(Data),1,ptrFile);
    fclose(ptrFile);
    int i=0;
    int fileListEntryIndex = -1;
    // find the file
    for(;i< 128 ; i++)
    {
        if( 0 == strcmp(stFileList.list[i].fileName,srcFileName))
        {
            fileListEntryIndex = i;
            break;
        }
    }
    if(fileListEntryIndex == -1 )
    {
        // File is not found return with warning message.
        printf("[WARNING]: File: %s does not exists.\n",srcFileName);
        return;
    }
    unsigned int currentFATEntry = stFileList.list[fileListEntryIndex].firstBlock;
    unsigned int fileSize =  stFileList.list[fileListEntryIndex].size;
    unsigned int blockSize;
    // calculate block size.
    if(fileSize % 512 != 0)
    {
        blockSize = (fileSize / 512)  + 1 ;
    }
    else
    {
        blockSize = fileSize / 512;
    }
    ptrFile = fopen(destPath,"w");
    unsigned int sizeToRead = 0;
    // write all of the blocks to the destination path.
    while(blockSize > 0)
    {
        memset(buffer.data,0,CHUNK_SIZE);
        if(fileSize > CHUNK_SIZE)
        {
            sizeToRead = CHUNK_SIZE;
        }
        else
        {
            sizeToRead = fileSize;
        }
        fileSize -= CHUNK_SIZE;
        memcpy(buffer.data,stData.list[currentFATEntry].data,CHUNK_SIZE);
        fwrite(buffer.data,sizeToRead,1,ptrFile);
        currentFATEntry = convertToBigEndian(stFAT.list[currentFATEntry].entry);
        blockSize--;
    }
    fclose(ptrFile);
}
void Delete(const char* disk, const char* filename)
{

    FAT stFAT;
    FileList  stFileList;
    ptrFile = fopen(disk,"w+");
    fread(&stFAT,sizeof(FAT),1,ptrFile);
    fread(&stFileList,sizeof (FileList),1,ptrFile);
    int i =1 ;
    int fileIndex = -1 ;
    for(; i <128 ; i++)
    {
        if( 0 == strcmp(stFileList.list[i].fileName,filename))
        {
            // then file is found
            fileIndex = i;
            break;
        }
    }
    if(fileIndex == -1 )
    {
        // File is not found return with warning message.
        printf("[WARNING]: File: %s does not exists.\n",filename);
        return;
    }
    int firstFATEntry = stFileList.list[fileIndex].firstBlock;
    unsigned int fileSize =  stFileList.list[fileIndex].size;
    unsigned int blockSize;
    if(fileSize % 512 != 0)
    {
        blockSize = fileSize / 512  + 1 ;
    }
    else
    {
        blockSize = fileSize / 512;
    }
    // delete entry from the file list
    memset(&( stFileList.list[fileIndex].fileName),0,248);
    stFileList.list[fileIndex].fileName[0] = '\0';
    stFileList.list[fileIndex].firstBlock = 0;
    stFileList.list[fileIndex].size = 0;
    // Clear FAT
    unsigned int currentFATEntry = convertToBigEndian(stFAT.list[firstFATEntry].entry);
    unsigned int nextFATEntry;
    if(currentFATEntry == 0xFFFFFFFF)
    {
    stFAT.list[firstFATEntry].entry = 0;
    }
    else
    {
        do
        {
            nextFATEntry = convertToBigEndian(stFAT.list[currentFATEntry].entry);
            stFAT.list[currentFATEntry].entry = 0;
            currentFATEntry = nextFATEntry;

        } while (currentFATEntry != 0xFFFFFFFF);
    }
    fseek(ptrFile,0,SEEK_SET);
    fwrite(&stFAT,sizeof(FAT),1,ptrFile);
    fwrite(&stFileList,sizeof(FileList),1,ptrFile);
    fclose(ptrFile);
}

// List the existing files inside the disk provided.
void List(const char* disk)
{
    // Open the disk in reading mode.
    ptrFile = fopen(disk,"r");
    FileListEntry currentFileListEntry;
    // set ptr to start of file list
    fseek(ptrFile,sizeof(FAT),SEEK_SET);
    printf("File Name   File Size\n");
    int i=0;
    for(;i< 128 ; i++)
    {
        // Read file list entries one by one, if firstBlock and size is not equal to zero,
        // then we can say that there is a file. after that file name and size is printed.
        fread(&currentFileListEntry,sizeof(FileListEntry),1,ptrFile);
        if(currentFileListEntry.firstBlock != 0 && currentFileListEntry.size != 0)
        {
            printf("%-10s   %-10d\n",currentFileListEntry.fileName,currentFileListEntry.size);
        }
        else {}
    }
    fclose(ptrFile);
}
void Defragment(const char* disk)
{
    FAT stoldFAT,stnewFAT;
    FileList  stoldFileList,stnewFileList;
    Data stoldData,stnewData;

    memset(&stnewFAT,0,sizeof(FAT));
    memset(&stnewFileList,0,sizeof(FileList));
    memset(&stnewData,0,sizeof(Data));
    stnewFAT.list[0].entry = 0xFFFFFFFF;

    ptrFile = fopen(disk,"r");
    fread(&stoldFAT,sizeof(FAT),1,ptrFile);
    fread(&stoldFileList,sizeof (FileList),1,ptrFile);
    fread(&stoldData,sizeof(Data),1,ptrFile);
    fclose(ptrFile);


    int i=0;
    int existingFileArray[128];
    int fileCount = 0;
    for(;i<128;i++)
    {
        if(stoldFileList.list[i].size != 0 && stoldFileList.list[i].firstBlock != 0)
        {
            fileCount++;
            existingFileArray[fileCount-1] = i;
        }
    }
    int fileIndex = 0;
    unsigned int currentNewFATBlock = 1;
    unsigned int currentOldFATBlock = 1;
    unsigned int blockSize;
    unsigned int fileSize;
    int j;
    for(i=0; i < fileCount ; i++)
    {
        fileIndex = existingFileArray[i];
        // update file list
        stnewFileList.list[i].size = stoldFileList.list[fileIndex].size;
        stnewFileList.list[i].firstBlock = currentNewFATBlock;
        currentOldFATBlock = stoldFileList.list[fileIndex].firstBlock;
        memcpy(stnewFileList.list[i].fileName,stoldFileList.list[fileIndex].fileName,248);
        fileSize = stnewFileList.list[i].size;
        if(fileSize % 512 != 0)
        {
            blockSize = fileSize / 512  + 1 ;
        }
        else
        {
            blockSize = fileSize / 512;
        }
        for(j=0; j <blockSize ; j++)
        {
            memcpy(stnewData.list[currentNewFATBlock].data,stoldData.list[currentOldFATBlock].data,sizeof(DataEntry));
            if(j == blockSize-1)
            {
                stnewFAT.list[currentNewFATBlock].entry = 0xFFFFFFFF;
            }
            else
            {
                stnewFAT.list[currentNewFATBlock].entry = convertToLittleEndian((currentNewFATBlock+1));
                currentNewFATBlock++;
                currentOldFATBlock = convertToBigEndian(stoldFAT.list[currentOldFATBlock].entry);
            }
        }
    }
    ptrFile = fopen(disk,"w");
    fwrite(&stnewFAT,sizeof(FAT),1,ptrFile);
    fwrite(&stnewFileList,sizeof(FileList),1,ptrFile);
    fwrite(&stnewData,sizeof(Data),1,ptrFile);
    fclose(ptrFile);

}