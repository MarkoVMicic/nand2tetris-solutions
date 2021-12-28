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


void GrowVMString(vm_string *VMString)
{
    Assert(VMString->MemorySize <= VMString->CurrentLength);
    // NOTE(Marko): Double size of buffer until the buffer is large enough. 
    while(VMString->MemorySize <= VMString->CurrentLength)
    {
        VMString->MemorySize *= 2;
    }
    VMString->Contents = 
        (char *)realloc((void *)VMString->Contents, 
                        VMString->MemorySize);

    if(VMString->Contents == NULL)
    {
        printf("realloc failed!\n");
        InvalidCodePath;
    }

}


void FreeVMString(vm_string *VMString)
{
    free(VMString->Contents);
    free(VMString);
}


vm_string *AllocateVMString(uint32 MemoryBlockSize)
{
    vm_string *Result = (vm_string *)malloc(sizeof(vm_string));

    Result->Contents = (char *)malloc(MemoryBlockSize*sizeof(char));
    if(Result->Contents)
    {
        Result->MemorySize = MemoryBlockSize;
        Result->CurrentLength = 0;
        Result->Contents[MemoryBlockSize-1] = '\0';
    }
    else
    {
        printf("malloc failed! \n");
        InvalidCodePath;
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

