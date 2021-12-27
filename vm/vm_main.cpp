#include "vm_main.h"
#include "vm_string.h"
#include "translate_vm.h"

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
        // TODO(Marko): Check for null termination!
        vm_string VMInput = {0};
        VMInput.Contents = InputFileContents.Contents;
        VMInput.CurrentLength = InputFileContents.Length;
        VMInput.MemorySize = VMInput.CurrentLength; 

        vm_string ASMOutputBuffer = 
            AllocateVMString(DEFAULT_INITIAL_VM_STRING_SIZE);

        TranslateVMInstructionsToAsm(&VMInput, &ASMOutputBuffer);
    }

    FreeEntireFileMemory(InputFileContents); 

    return(0);
}