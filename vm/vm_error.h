
#if !defined(VM_ERROR_H)
#include "vm_string.h"


#define INITIAL_ERROR_ALLOCATION_AMOUNT 16

struct vm_error_list
{
    vm_string *ErrorTypes;
    uint32 *ErrorLines;
    uint32 ErrorsWrittenCount;
    uint32 MaxErrorCount;
    uint32 CurrentLineCount;
};

void AddErrorToErrorList(vm_error_list *ErrorList, 
                         vm_string *Error);

vm_error_list *InitializeErrorList(uint32 MaxErrorCount, 
                                   uint32 ErrorVMStringSize);

void PrintErrorsToConsole(vm_error_list *ErrorList);

#define VM_ERROR_H
#endif
