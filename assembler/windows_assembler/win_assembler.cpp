#include <stdio.h>
#include <windows.h>
#include "win_assembler.h"

// NOTE(Marko): Doing a unity build. 
#include "process_symbols.cpp"
#include "preprocess.cpp"
#include "parse_instructions.cpp"


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
    debug_read_file_result InputFileReadResult = 
        DEBUGReadEntireFile(InputFilePath);

    if(InputFileReadResult.Contents != 0)
    {


        asm_string AsmString;
        AsmString.Contents = (char *)InputFileReadResult.Contents;
        AsmString.Length = InputFileReadResult.ContentsSize;

       

        variable_table PredefinedVariableTable = 
            CreatePredefinedVariableTable();

        // NOTE(Marko): This is allocated in PreprocessAsmString(), because we 
        //              don't know how large to make the allocation until 
        //              we've done 1 pass through the asm file. 
        variable_table UserDefinedVariableTable = {0};

        // We need the line count to allocate the machine code asm_string.
        uint32 LineCount = 0;


#if 1
        OutputDebugString("AsmString contents: \n");
        OutputDebugString(AsmString.Contents);
#endif
        PreprocessAsmString(&AsmString, 
                            &PredefinedVariableTable,
                            &UserDefinedVariableTable,
                            &LineCount);



        // NOTE(Marko): Allocate the Machine Code string. Note that we don't 
        //              need to add an extra byte to the allocation for null 
        //              termination because DEBUGWriteEntireFile() takes in a 
        //              length and writes memory (i.e. it takes a void *)
        asm_string MachineCodeAsmString = 
            InitializeAsmString(LineCount * MACHINE_CODE_LINE_LENGTH);

        ParseInstructions(&AsmString, 
                          &MachineCodeAsmString, 
                          &UserDefinedVariableTable);

        DEBUGWriteEntireFile(OutputFilePath, 
                             AsmString.Length, 
                             (void *)AsmString.Contents);

        // NOTE(Marko): Memory Cleanup
        DEBUGFreeFileMemory(InputFileReadResult.Contents);
        // NOTE(Marko): Although we don't need the PredefinedVariableTable 
        //              after the second pass (which happens in PreProcess()), 
        //              we wait until the end of the program to free it 
        //              because freeing it requires quite a lot of calls to 
        //              VirtualFree() which might be slow. Of course, this is 
        //              wishy washy coz I haven't actually profiled it. 
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