#include "vm_string.h"


vm_string AllocateVMString(uint32 MemoryBlockSize)
{
    vm_string Result = {0};

    Result.Contents = (char *)malloc(MemoryBlockSize*sizeof(char));
    if(Result.Contents)
    {
        Result.MemorySize = MemoryBlockSize;
    }

    return(Result);
}

