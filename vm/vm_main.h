#if !defined(VM_MAIN_H)

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: We are putting these #define in the code to make the code more 
//       searchable. Now we can more easily search for global variables, 
//       locally-persistent variables, and internal functions much more easily 
//       than if these three vastly different types were all indicated with 
//       the same static keyword. 
#define internal static    
#define local_persist static
#define global_variable static 

typedef int8_t int8; 
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

// NOTE: unsigned char is the same as an 8-bit integer. 
typedef uint8_t uint8;  
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#if VM_DEBUG
    // NOTE: Assert macro -> Write to the NULL pointer if Expression is false. 
    //       This is platform independent. 
    // TODO: Complete Assertion Macro
    #define Assert(Expression) if(!(Expression)){* (int *)0 = 0;}
#else
    #define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath");

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

//
// NOTE(Marko): Simple File I/O
//

struct read_file_result
{
    char *Contents;
    uint32 Length;
};

read_file_result ReadEntireFile(char *FileName)
{
    read_file_result Result = {0};

    FILE *FileHandle = fopen(FileName, "r");
    if(FileHandle)
    {
        // NOTE(Marko): Get file size
        // NOTE(Marko): This is not a robust way to get file size. It works 
        //              for our input though. In particular it won't work for 
        //              files of size >4GB
        if(fseek(FileHandle, 0, SEEK_END) == 0)
        {
            // NOTE(Marko): fseek() returns 0 upon success
            Result.Length = ftell(FileHandle);
        }
        else
        {
            printf("fseek() failed\n");
            Result.Length = 0;
        }

        // NOTE(Marko): Reset to beginning of file
        if((Result.Length != 0) && (fseek(FileHandle, 0, SEEK_SET) == 0))
        {
            // NOTE(Marko): fseek() returns 0 upon success
            // NOTE(Marko): Add 1 to buffer size because fread() won't null 
            //              terminate. 
            Result.Contents = 
                (char *)malloc((Result.Length + 1) * sizeof(char));
            if(Result.Contents)
            {
                // TODO(Marko): Figure out how fread() works. 
                fread(Result.Contents, 1, Result.Length, FileHandle);
                Result.Contents[Result.Length] = '\0';
            }
            else
            {
                printf("malloc() failed\n");
                Result.Length = 0;
            }
        }
        else
        {
            printf("fseek() failed\n");
            Result.Length = 0;
        }
    }
    else
    {
        printf("fopen() failed. \n");
        Result = {0};
    }

    fclose(FileHandle);

    return(Result);
}


void FreeEntireFileMemory(read_file_result FileMemory)
{
    if((FileMemory.Length > 0) && (FileMemory.Contents != NULL))
    {
        free(FileMemory.Contents);
        FileMemory.Length = 0;
    }
}

#define VM_MAIN_H
#endif