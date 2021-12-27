#include "vm_string.h"
#include "vm_main.h"

bool32 VMStringsAreEqual(vm_string *VMStringA, 
                         vm_string *VMStringB)
{
    bool32 Result = true;
    if(VMStringA->CurrentLength != VMStringB->CurrentLength)
    {
        // NOTE(Marko): Early return for unequal lengths
        Result = false;
        return(Result);
    }
    for(uint32 StringIndex = 0; 
        StringIndex < VMStringA->CurrentLength; 
        StringIndex++)
    {
        if(VMStringA->Contents[StringIndex] != 
            VMStringB->Contents[StringIndex])
        {
            Result = false;
            break;
        }
    }

    return(Result);
}


bool32 VMStringsAreEqual(vm_string *VMStringA, 
                         char *StringB, 
                         uint32 StringBLength)
{
    vm_string VMStringB;
    VMStringB.Contents = StringB;
    VMStringB.CurrentLength = StringBLength;
    // NOTE(Marko): Presumably there is a null-terminator! 
    VMStringB.MemorySize = StringBLength+1;

    bool32 Result = VMStringsAreEqual(VMStringA, &VMStringB);

    return(Result);
}


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

