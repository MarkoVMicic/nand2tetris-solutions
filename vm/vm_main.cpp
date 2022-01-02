#include "vm_main.h"
#include "vm_string.h"
#include "vm_tokens.h"
#include "translate_vm.h"
#include "vm_error.h"

// NOTE(Marko): Global Variable for program name. 
vm_string GlobalProgramName = {0};


void WriteVMStringToFile(vm_string *VMString, char *FileName)
{
    FILE *WriteFileHandle = fopen(FileName, "w");
    fwrite(VMString->Contents, 1, VMString->CurrentLength, WriteFileHandle);
    fclose(WriteFileHandle);
}

read_file_result ReadEntireFile(char *FileName,
                                vm_error_list *ErrorList)
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
            vm_string Error = ConstructVMStringFromCString("fseek() failed while trying to seek to end of file.");
            AddErrorToErrorList(ErrorList, &Error);
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
                vm_string Error = ConstructVMStringFromCString("malloc() failed while attempting to allocate memory for the file.");
                AddErrorToErrorList(ErrorList, &Error);
                Result.Length = 0;
            }
        }
        else
        {
            vm_string Error = ConstructVMStringFromCString("fseek() failed while seeking back to the beginning of file.");
            AddErrorToErrorList(ErrorList, &Error);
            Result.Length = 0;
        }
    }
    else
    {
        vm_string Error = ConstructVMStringFromCString("fopen() failed while trying to open the file.");
        AddErrorToErrorList(ErrorList, &Error);
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
    vm_error_list *ErrorList = 
        InitializeErrorList(INITIAL_ERROR_ALLOCATION_AMOUNT,
                            DEFAULT_INITIAL_VM_STRING_SIZE);
    if(argc != 3)
    {
        vm_string Error = ConstructVMStringFromCString("Incorrect number of args supplied to program");
        AddErrorToErrorList(ErrorList, &Error);
    }
    else
    {
        char *InputFileName = argv[1];
        char *OutputFileName = argv[2];        
        read_file_result InputFileContents = ReadEntireFile(InputFileName,
                                                            ErrorList);
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
                AllocateVMString(INITIAL_ASM_OUTPUT_STRING_SIZE);

            instruction_counts InstructionCounts = {0};

            vm_string *ASMInstructions = 
                AllocateVMString(DEFAULT_INITIAL_VM_STRING_SIZE);

            vm_tokens *VMTokens = AllocateVMTokens(MAX_VM_TOKEN_COUNT, 
                                                   DEFAULT_INITIAL_VM_STRING_SIZE);

            TranslateVMInstructionsToASM(&VMInput, 
                                         ASMOutputBuffer, 
                                         &InstructionCounts,
                                         ASMInstructions,
                                         VMTokens);

            WriteVMStringToFile(ASMOutputBuffer, OutputFileName);
            FreeVMString(ASMOutputBuffer);
            FreeVMString(ASMInstructions);
            FreeVMTokens(VMTokens);
            FreeProgramName(GlobalProgramName);
        }
        else
        {
            vm_string Error = ConstructVMStringFromCString("ReadEntireFile returned a read_file_result with length 0 (empty file?)");
            AddErrorToErrorList(ErrorList, &Error);
        }
        FreeEntireFileMemory(InputFileContents); 
    }
    
    if(ErrorList->ErrorsWrittenCount > 0)
    {
        PrintErrorsToConsole(ErrorList);
    }


    return(0);
}