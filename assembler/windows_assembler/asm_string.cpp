#include "asm_string.h"


internal asm_string InitializeAsmString(uint32 Length)
{
    asm_string Result = {0};
    Result.Contents = (char *)VirtualAlloc(0, 
                                           Length*sizeof(char), 
                                           MEM_COMMIT | MEM_RESERVE, 
                                           PAGE_READWRITE);           
    if(Result.Contents)
    {
        Result.Length = Length;
    }

    return(Result);
}


internal void FreeAsmString(asm_string *AsmString)
{
    VirtualFree(AsmString->Contents, 0, MEM_RELEASE);
    VirtualFree(AsmString, 0, MEM_RELEASE);
}


inline void CopyString(char *SourceString, 
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


inline void CopyAsmString(asm_string *SourceAsmString,
                          asm_string *DestAsmString)
{
    CopyString(SourceAsmString->Contents,
               SourceAsmString->Length,
               DestAsmString->Contents,
               DestAsmString->Length);
}


inline bool32 AsmStringsMatch(asm_string *StringA, 
                              asm_string *StringB)
{
    bool32 Result = true;
    if(StringA->Length == StringB->Length)
    {
        for(uint32 i = 0; i < StringA->Length; i++)
        {
            if(StringA->Contents[i] != StringB->Contents[i])
            {
                Result = false;
                break;
            }
        }
    }
    else
    {
        Result = false;
    }

    return(Result);
}


internal void DebugPrintAsmString(asm_string *AsmString)
{
    // NOTE(Marko): IMPORTANT WARNING: OutputDebugString has a limit of ~4KB, 
    //                                 and will just silently not output 
    //                                 everything if the string is too 
    //                                 large.       
    OutputDebugString("Printing AsmString...\n");
    OutputDebugString("AsmString Length: ");
    DebugPrintUInt32(AsmString->Length);
    char *NullTerminatedAsmString = 
        (char *)VirtualAlloc(0, 
                            (AsmString->Length+1)*sizeof(char), 
                            MEM_COMMIT|MEM_RESERVE, 
                            PAGE_READWRITE);
    CopyString(AsmString->Contents, 
               AsmString->Length, 
               NullTerminatedAsmString, 
               AsmString->Length);
    NullTerminatedAsmString[AsmString->Length] = '\0';

    OutputDebugString(NullTerminatedAsmString);
    OutputDebugString("\n");

    printf("Printing AsmString...\n");
    printf("AsmString Length: %d\n", AsmString->Length);
    printf("%s\n",NullTerminatedAsmString);
    printf("\n");

    VirtualFree(NullTerminatedAsmString, 0, MEM_RELEASE);
}