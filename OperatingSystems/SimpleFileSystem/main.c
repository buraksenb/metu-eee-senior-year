//
// Created by burak on 08-Jun-21.
//
#include "FileSystem.h"

int main(int argc, char *argv[])
{
    if(strcmp(argv[2],"-format") == 0)
    {
        Format(argv[1]);
    }
    else if(strcmp(argv[2],"-write") == 0)
    {
        Write(argv[1],argv[3],argv[4]);
    }
    else if(strcmp(argv[2],"-read") == 0)
    {
        Read(argv[1],argv[3],argv[4]);
    }

    else if(strcmp(argv[2],"-delete") == 0)
    {
        Delete(argv[1],argv[3]);
    }
    else if(strcmp(argv[2],"-list") == 0)
    {
        List(argv[1]);
    }
    else if(strcmp(argv[2],"-defragment") == 0)
    {
        Defragment(argv[1]);
    }
    else
    {
        printf("Please enter a valid operation!\n");
    }
    return 0;
}
