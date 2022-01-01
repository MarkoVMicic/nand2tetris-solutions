#include "vm_main.h"
#include "vm_string.h"
#include "translate_vm.h"

global_variable vm_string GlobalProgramName = {0};



void WriteVMStringToFile(vm_string *VMString, char *FileName)
{
    FILE *WriteFileHandle = fopen(FileName, "w");
    fwrite(VMString->Contents, 1, VMString->CurrentLength, WriteFileHandle);
    fclose(WriteFileHandle);
}

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
        uint32 FileSize = 0;
        if(fseek(FileHandle, 0, SEEK_END) == 0)
        {
            // NOTE(Marko): fseek() returns 0 upon success
            FileSize = ftell(FileHandle);
        }
        else
        {
            printf("fseek() failed\n");
            FileSize = 0;
        }

        // NOTE(Marko): Reset to beginning of file
        if((FileSize != 0) && (fseek(FileHandle, 0, SEEK_SET) == 0))
        {
            // NOTE(Marko): fseek() returns 0 upon success
            // NOTE(Marko): Add 1 to buffer size because fread() won't null 
            //              terminate. 
            Result.Contents = 
                (char *)malloc((FileSize + 1) * sizeof(char));
            if(Result.Contents)
            {
                // NOTE(Marko): fread() returns the number of actually read 
                //              elements. Since it appears that the CRT 
                //              removes the carriage returns, this means that 
                //              StringLength may be less than FileSize. 
                uint32 StringLength = fread(Result.Contents, 
                                            1, 
                                            FileSize, 
                                            FileHandle);
                Result.Contents[StringLength] = '\0';
                Result.Length = StringLength;
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

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: supply the *.vm file to be translated as the first\n");
        printf("argument, and supply the target output file as the second\n");
        printf("argument.\n");

        return(1);
    }
    char *InputFileName = argv[1];
    char *OutputFileName = argv[2];

    read_file_result InputFileContents = ReadEntireFile(InputFileName);
    if(InputFileContents.Length > 0)
    {
        GlobalProgramName = 
            RetrieveProgramNameFromInputFileName(InputFileName);
        // TODO(Marko): Check for null termination!
        vm_string VMInput = {0};
        VMInput.Contents = InputFileContents.Contents;
        VMInput.CurrentLength = InputFileContents.Length;
        VMInput.MemorySize = VMInput.CurrentLength; 

        vm_string *ASMOutputBuffer = 
            AllocateVMString(DEFAULT_INITIAL_VM_STRING_SIZE);

        instruction_counts InstructionCounts = {0};

        // TODO(Marko): Allocate ASMInstructions vm_string once here and free here. This prevents multiple calls to malloc() and free() for a piece of memory that is easily reusable. 
        // TODO(Marko): Allocate VMTokens once here and free here. This prevents multiple calls to malloc() and free() for a piece of memory that is easily reusable. 

        TranslateVMInstructionsToASM(&VMInput, 
                                     ASMOutputBuffer, 
                                     &InstructionCounts);


        WriteVMStringToFile(ASMOutputBuffer, OutputFileName);
        FreeVMString(ASMOutputBuffer);
        FreeProgramName(GlobalProgramName);
    }
    FreeEntireFileMemory(InputFileContents); 

    return(0);
}