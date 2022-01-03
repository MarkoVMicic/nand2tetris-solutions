#include "vm_main.h"
#include "vm_string.h"
#include "vm_tokens.h"
#include "translate_vm.h"
#include "vm_error.h"

//
// NOTE(Marko): Global Variable for program name. 
//
vm_string GlobalProgramName = {0};

//
// NOTE(Marko): Functions that fill in the global variable GlobalProgramName
//
vm_string RetrieveProgramNameFromInputFileName(const char *InputFileName,
                                               vm_error_list *ErrorList)
{
    vm_string Result = {0};
    uint32 InputFileNameLength = StringLength(InputFileName);

    // NOTE(Marko): "X:\Y\abcdefg\input_files\MyVmProgram.vm" -- we wish to 
    //              extract "MyVmProgram.vm"

    // NOTE(Marko): Check filename extension.
    if(InputFileName[InputFileNameLength-3] != '.' ||
       InputFileName[InputFileNameLength-2] != 'v' ||
       InputFileName[InputFileNameLength-1] != 'm')
    {
        vm_string Error = ConstructVMStringFromCString("Input file name has wrong extension. Expected *.vm");
        AddErrorToErrorList(ErrorList, &Error);
    }
    else
    {
        // NOTE(Marko): This is for windows, which demarcates using backslash!
        uint32 ProgramNameBeginIndex = 0;
        for(uint32 Index = InputFileNameLength-1; Index >= 0; Index--)
        {
            if(InputFileName[Index] == '\\')
            {
                ProgramNameBeginIndex = Index+1;
                break;
            }
            Result.CurrentLength++;
        }
        // NOTE(Marko): remove the file extension ".vm";
        Result.CurrentLength -= 3;

        Result.MemorySize = Result.CurrentLength + 1;

        Result.Contents = (char *)malloc(Result.MemorySize*sizeof(char));
        if(Result.Contents != NULL)
        {
            for(uint32 Index = 0; Index < Result.CurrentLength; Index++)
            {
                Result.Contents[Index] = 
                    InputFileName[ProgramNameBeginIndex+Index];
            }
            Result.Contents[Result.CurrentLength] = '\0';                
        }
        else
        {
            vm_string Error = ConstructVMStringFromCString("malloc() failed while trying to allocate space for the program name.");
            AddErrorToErrorList(ErrorList, &Error);
            Result.CurrentLength = 0;
            Result.MemorySize = 0;
        }
    }
    return(Result);
}

void FreeProgramName(vm_string GlobalProgramName)
{
    free(GlobalProgramName.Contents);
}


//
// NOTE(Marko): Print Instruction Counts
//
void PrintInstructionCounts(instruction_counts *InstructionCounts)
{
    // NOTE(Marko): Push counts
    printf("PushArgumentCount: %d\n", InstructionCounts->PushArgumentCount);
    printf("PushLocalCount: %d\n", InstructionCounts->PushLocalCount);
    printf("PushStaticCount: %d\n", InstructionCounts->PushStaticCount);
    printf("PushConstantCount: %d\n", InstructionCounts->PushConstantCount);
    printf("PushThisCount: %d\n", InstructionCounts->PushThisCount);
    printf("PushThatCount: %d\n", InstructionCounts->PushThatCount);
    printf("PushPointerCount: %d\n", InstructionCounts->PushPointerCount);
    printf("PushTempCount: %d\n", InstructionCounts->PushTempCount);

    // NOTE(Marko): Pop Counts
    printf("PopArgumentCount: %d\n", InstructionCounts->PopArgumentCount);
    printf("PopLocalCount: %d\n", InstructionCounts->PopLocalCount);
    printf("PopStaticCount: %d\n", InstructionCounts->PopStaticCount);
    printf("PopThisCount: %d\n", InstructionCounts->PopThisCount);
    printf("PopThatCount: %d\n", InstructionCounts->PopThatCount);
    printf("PopPointerCount: %d\n", InstructionCounts->PopPointerCount);
    printf("PopTempCount: %d\n", InstructionCounts->PopTempCount);
    
    // NOTE(Marko): Arithmetic Command counts
    printf("AddCount: %d\n", InstructionCounts->AddCount);
    printf("SubCount: %d\n", InstructionCounts->SubCount);
    printf("NegCount: %d\n", InstructionCounts->NegCount);
    printf("EqCount: %d\n", InstructionCounts->EqCount);
    printf("GtCount: %d\n", InstructionCounts->GtCount);
    printf("LtCount: %d\n", InstructionCounts->LtCount);
    printf("AndCount: %d\n", InstructionCounts->AndCount);
    printf("OrCount: %d\n", InstructionCounts->OrCount);
    printf("NotCount: %d\n", InstructionCounts->NotCount);
}


//
// NOTE(Marko): File I/O
//
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
    instruction_counts InstructionCounts = {0};
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
                RetrieveProgramNameFromInputFileName(InputFileName,
                                                     ErrorList);
            if(GlobalProgramName.CurrentLength == 0)
            {
                // NOTE(Marko): If the program name had the wrong extension or 
                //              was unable to be heap allocated, then we can 
                //              simply replace it with some indicator that 
                //              this didn't work, but still generate valid 
                //              assembly. 
                GlobalProgramName = ConstructVMStringFromCString("<vm_file_not_found>");
            }
            // TODO(Marko): Check for null termination!
            vm_string VMInput = {0};
            VMInput.Contents = InputFileContents.Contents;
            VMInput.CurrentLength = InputFileContents.Length;
            VMInput.MemorySize = VMInput.CurrentLength; 

            vm_string *ASMOutputBuffer = 
                AllocateVMString(INITIAL_ASM_OUTPUT_STRING_SIZE);


            vm_string *ASMInstructions = 
                AllocateVMString(DEFAULT_INITIAL_VM_STRING_SIZE);

            vm_tokens *VMTokens = AllocateVMTokens(MAX_VM_TOKEN_COUNT, 
                                                   DEFAULT_INITIAL_VM_STRING_SIZE,
                                                   ErrorList);

            TranslateVMInstructionsToASM(&VMInput, 
                                         ASMOutputBuffer, 
                                         &InstructionCounts,
                                         ASMInstructions,
                                         VMTokens,
                                         ErrorList);

            WriteVMStringToFile(ASMOutputBuffer, OutputFileName);
            FreeVMString(ASMOutputBuffer);
            FreeVMString(ASMInstructions);
            FreeVMTokens(VMTokens);
            if(!VMStringsAreEqual(&GlobalProgramName, 
                &ConstructVMStringFromCString("<vm_file_not_found>")))
            {
                FreeProgramName(GlobalProgramName);
            }
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
    
    PrintInstructionCounts(&InstructionCounts);

    return(0);
}