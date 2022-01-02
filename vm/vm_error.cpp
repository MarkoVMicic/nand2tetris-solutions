#include "vm_error.h"
#include "vm_string.h"

void AddErrorToErrorList(vm_error_list *ErrorList, 
                         vm_string *Error)
{
    uint32 ErrorIndex = ErrorList->ErrorsWrittenCount;
    ErrorList->ErrorsWrittenCount++;
    if(ErrorList->ErrorsWrittenCount > ErrorList->MaxErrorCount)
    {
        // TODO(Marko): Either implement growing, or early return and 
        //              terminate the program printing all the errors obtained 
        //              until now. 
        InvalidCodePath;
    }
    ErrorList->ErrorTypes[ErrorIndex].CurrentLength = Error->CurrentLength;
    if(ErrorList->ErrorTypes[ErrorIndex].MemorySize <= 
        ErrorList->ErrorTypes[ErrorIndex].CurrentLength)
    {
        GrowVMString(&ErrorList->ErrorTypes[ErrorIndex]);
    }
    CopyVMString(Error->Contents,
                 Error->CurrentLength,
                 ErrorList->ErrorTypes[ErrorIndex].Contents,
                 ErrorList->ErrorTypes[ErrorIndex].CurrentLength);
    ErrorList->ErrorTypes[ErrorIndex].Contents[ErrorList->ErrorTypes[ErrorIndex].CurrentLength] = '\0';
    ErrorList->ErrorLines[ErrorIndex] = ErrorList->CurrentLineCount;
}

vm_error_list *InitializeErrorList(uint32 MaxErrorCount, 
                                   uint32 ErrorVMStringSize)
{
    vm_error_list *Result = (vm_error_list *)malloc(sizeof(vm_error_list));

    Result->MaxErrorCount = MaxErrorCount;
    Result->ErrorTypes = 
        (vm_string *)malloc(Result->MaxErrorCount*sizeof(vm_string *));
    Result->ErrorLines = 
        (uint32 *)malloc(Result->MaxErrorCount*sizeof(uint32));
    Result->ErrorsWrittenCount = 0;
    Result->CurrentLineCount = 0;

    for(uint32 Index = 0; Index < Result->MaxErrorCount; Index++)
    {
        vm_string *CurrentVMString = 
            AllocateVMString(ErrorVMStringSize);
        Result->ErrorTypes[Index] = *CurrentVMString;
    }


    return(Result);
}
