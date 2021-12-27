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


void CopyVMString(char *SourceString,
                  uint32 SourceStringLength,
                  char *DestString,
                  uint32 DestStringLength)
{
    Assert(DestStringLength >= SourceStringLength);
    
    for(uint32 i = 0; i < SourceStringLength; i++)
    {
        DestString[i] = SourceString[i];
    }
}                  

