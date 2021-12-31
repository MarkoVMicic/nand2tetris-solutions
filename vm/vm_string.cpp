#include "vm_string.h"
#include "vm_main.h"


inline bool32 IsCharNumber(char Char)
{
    bool32 Result = ((Char == '0') ||
                     (Char == '1') ||
                     (Char == '2') ||
                     (Char == '3') ||
                     (Char == '4') ||
                     (Char == '5') ||
                     (Char == '6') ||
                     (Char == '7') ||
                     (Char == '8') ||
                     (Char == '9'));

    return(Result);
}


// TODO(Marko): Find a place to put this Pow function
// TODO(Marko): Can make this a lot better using square and multiply algorithm 
//              on the bits than this multiply but it's probably not that 
//              important
inline uint32 Pow(uint32 Base, uint32 Exponent)
{
    uint32 Result = 1;
    if(Base == 0 && Exponent == 0)
    {
        // NOTE(Marko): This is undefined, we shouldn't be calling into this 
        //              function like this! 
        InvalidCodePath;
    }
    else if(Base == 0)
    {
        Result = 0;
    }
    else if(Exponent == 0)
    {
        Result = 1;
    }
    else if(Exponent == 1)
    {
        Result = Base;
    }
    else if(Exponent == 2)
    {
        Result = Base*Base;
    }  
    else if(Exponent == 3)
    {
        Result = Base*Base*Base;
    }  
    else if(Exponent == 4)
    {
        Result = Base*Base*Base*Base;
    }      
    else
    {
        for(uint32 i = 0; i < Exponent; i++)
        {
            Result *= Base;
        }
    }

    return(Result);
}

uint32 VMStringToUInt32(vm_string *VMString)
{
    uint32 Result = 0;
    for(uint32 Index = 0; Index < VMString->CurrentLength; Index++)
    {
        if(!IsCharNumber(VMString->Contents[Index]))
        {
            InvalidCodePath;
        }
        Result += (uint32)(VMString->Contents[Index] -'0') * 
                  Pow(10,(VMString->CurrentLength-1) - Index);
    }

    return(Result);
}

vm_string UInt32ToVMString(uint32 UInt32)
{
    vm_string Result;
    uint32 N = UInt32;
    uint32 DigitCount = 0;
    do
    {
        DigitCount++;
        N /= 10;
    } while(N > 0);

    Result.CurrentLength = DigitCount;
    Result.MemorySize = DigitCount+1;
    Result.Contents = (char *)malloc((DigitCount+1) * sizeof(char));
    uint32 NumIndex = DigitCount-1;
    do
    {
        Result.Contents[NumIndex] = (char)((UInt32 % 10) + '0');
        UInt32 /= 10;
        NumIndex--;

    } while(UInt32 > 0);

    Result.Contents[DigitCount] = '\0';

    return Result; 
}


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

