#include <stdio.h>
#include <windows.h>
#include "win_assembler.h"

// NOTE(Marko): Doing a unity build. 
#include "process_symbols.cpp"
#include "preprocess.cpp"


// 
// NOTE(Marko): File Read and File Write functions
//

DEBUG_FREE_FILE_MEMORY(DEBUGFreeFileMemory)
{
    if(Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

DEBUG_READ_ENTIRE_FILE(DEBUGReadEntireFile)
{
    debug_read_file_result Result = {};
    HANDLE FileHandle = CreateFileA(Filename,
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    0,
                                    OPEN_EXISTING,
                                    0,
                                    0);

    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize; 
        if(GetFileSizeEx(FileHandle, &FileSize)) 
        {   
            uint32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart); 
            Result.Contents = VirtualAlloc(0, 
                                  FileSize32, 
                                  MEM_RESERVE|MEM_COMMIT,
                                  PAGE_READWRITE);
            if(Result.Contents)
            {
                DWORD BytesRead;
                // NOTE: && is a sequential operator, it evaluates LHS first. 
                if(ReadFile(FileHandle,
                            Result.Contents,
                            FileSize32,
                            &BytesRead,
                            0) &&
                    (FileSize32 == BytesRead))
                {
                    // NOTE: File read successfully
                    Result.ContentsSize = FileSize32;
                }   
                else
                {
                    // NOTE: ReadFile() failed OR we read fewer than the bytes 
                    //       expected to be read. Free the memory immediately.
                    // TODO: Logging
                    DEBUGFreeFileMemory(Result.Contents);
                    Result.Contents = 0; 
                }
            }
            else
            {
                // TODO: Logging
                // NOTE: VirtualAlloc() failed
            }
        }
        else
        {
            // TODO: Logging
            // NOTE: GetFileSizeEx() failed
        } 
        CloseHandle(FileHandle);
    }
    else
    {
        // TODO: Logging
        // NOTE: CreateFileA() failed
    }
    

    return(Result);
}


DEBUG_WRITE_ENTIRE_FILE(DEBUGWriteEntireFile)
{
    bool32 Result = false;
    HANDLE FileHandle = CreateFileA(Filename,
                                    GENERIC_WRITE,
                                    0,
                                    0,
                                    CREATE_ALWAYS,
                                    0,
                                    0);

    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten;
        // NOTE: && is a sequential operator, it evaluates LHS first. 
        if(WriteFile(FileHandle,
                     Memory,
                     MemorySize,
                     &BytesWritten,
                     0))
        {
            // NOTE: File written successfully
            // NOTE: We check that BytesWritten == MemorySize. If not, we 
            //       haven't written all that we wanted to (or we may have 
            //       written even more than we wanted to for some reason).
            Result = (BytesWritten == MemorySize);
        }
        else
        {
            // TODO: Logging
            // NOTE: WriteFile() failed. 
        }   
        CloseHandle(FileHandle);
    }
    else
    {
        // TODO: Logging
        // NOTE: CreateFileA() failed
    }
    return(Result);
}



// TODO(Marko): Try to figure out how to use WinMain() properly here 
//              (for learning, not practicality)
int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: Supply the file path of the assembly file to be ");
        printf("translated into machine code as the first argument of ");
        printf("this program.\n ");
        printf("Supply the file path of the machine-code output file as the");
        printf("second argument of this program.\n ");
        printf("\n\n");
        printf("Aborting program...\n");
        return(1);
    }

    // char *ProgramName = argv[0];
    char *InputFilePath = argv[1];
    char *OutputFilePath = argv[2];

    // TODO(Marko): Check if the file extension is correct (i.e. *.asm). 
    debug_read_file_result InputFileReadResult = DEBUGReadEntireFile(InputFilePath);

    if(InputFileReadResult.Contents != 0)
    {
        // NOTE(Marko): Here, we create two blocks of memory of identical size: one for OldAsmString and one for NewAsmString. For each pass through the string of assembly instructions, we will selectively copy over characters from OldAsmString to NewAsmString. At the end of each pass, we will swap them (i.e. NewAsmString becomes OldAsmString, and vice versa). In this way, we can easily manage the editing of the strings without having to worry about multiple allocations and frees. Indeed the only thing we need to allocate on the heap are two strings, of equal size. Since we remove all the comments and whitespace, it's unlikely that we will ever need more space. And since asm_string keeps track of length, it's easy to find out when we've come to the end of any of those strings even after extensive editing.  
        // TODO: Create the predefined variable table here? Or not... consider it. 
        asm_string OldAsmString;
        OldAsmString.Contents = (char *)InputFileReadResult.Contents;
        OldAsmString.Length = InputFileReadResult.ContentsSize;

        asm_string NewAsmString;
        NewAsmString.Contents = 
            (char *)VirtualAlloc(0, 
                                 OldAsmString.Length*sizeof(char), 
                                 MEM_COMMIT | MEM_RESERVE, 
                                 PAGE_READWRITE);
        // NOTE(Marko): NewAsmString starts off empty, but we initialize it 
        //              with the same length as OldAsmString. When we 
        //              selectively copy over chars from OldAsmString to 
        //              NewAsmString for the first time, we will decrease the 
        //              length accordingly.
        NewAsmString.Length = OldAsmString.Length;

        variable_table PredefinedVariableTable = 
            CreatePredefinedVariableTable();

        // NOTE(Marko): This is allocated in PreprocessAsmString(), because we 
        //              don't know how large to make the allocation until 
        //              we've done 1 pass through the asm file. 
        variable_table UserDefinedVariableTable = {0};

        // We need the line count to allocate the machine code asm_string.
        uint32 LineCount = 0;


#if 1
        OutputDebugString("OldAsmString contents: \n");
        OutputDebugString(OldAsmString.Contents);
#endif
        PreprocessAsmString(&OldAsmString, 
                            &NewAsmString, 
                            &PredefinedVariableTable,
                            &UserDefinedVariableTable,
                            &LineCount);

        DEBUGWriteEntireFile(OutputFilePath, 
                             OldAsmString.Length, 
                             (void *)OldAsmString.Contents);

        // NOTE(Marko): Memory Cleanup
        DEBUGFreeFileMemory(InputFileReadResult.Contents);
        VirtualFree(NewAsmString.Contents, 0, MEM_RELEASE);
        FreeVariableTable(&PredefinedVariableTable);
        FreeVariableTable(&UserDefinedVariableTable);
    }
    else
    {
        // TODO(Marko): Better logging
        // NOTE(Marko): InputFileReadResult has no contents!?
        printf("Error: Filesize is 0. \n");
    }



    return(0);
}