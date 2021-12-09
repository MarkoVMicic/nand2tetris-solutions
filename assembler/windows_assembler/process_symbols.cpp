#include "process_symbols.h"

// TODO(Marko): Figure out what kind of memory strategy we are going to use.
//              Right now this won't work because I haven't allocated memory 
//              for anything. 
//
//              !!! IDEA(Marko): 
//              Perhaps after removing all the comments and whitespace, we can
//              then count the number of variables that are used in the 
//              program, and use that information to allocate the entire 
//              variable table at once. This will prevent us from littering 
//              the code with mallocs which can cause heap fragmentation (and 
//              is just generally quite slow). We can even count the size of 
//              all the strings and just allocate one mega struct with strings 
//              and an array of string lengths etc so that we can easily find 
//              the variable strings we need.  

#define PREDEFINED_VAR_COUNT 23

internal bool32 IsInVariableTable(variable_table *VariableTable, 
                                  asm_string *AsmString)
{                                  
    bool32 Result = false; 
    for(uint32 i = 0; i < VariableTable->Size; i++)
    {
        if(AsmStringsMatch(&VariableTable->VariableNames[i], AsmString))
        {
            Result = true;
            break;
        }
    }

    return(Result);
}


inline void AddVariableToVariableTable(variable_table *VariableTable,
                                       uint32 Index, 
                                       char *VariableName, 
                                       uint32 VariableNameLength, 
                                       uint16 VariableAddress)
{
    VariableTable->VariableNames[Index].Contents = 
        (char *)VirtualAlloc(0, 
                             VariableNameLength*sizeof(char), 
                             MEM_RESERVE | MEM_COMMIT, 
                             PAGE_READWRITE);
    CopyString(VariableName, 
               VariableNameLength, 
               VariableTable->VariableNames[Index].Contents, 
               VariableNameLength);

    VariableTable->VariableNames[Index].Length = VariableNameLength;

    VariableTable->VariableAddresses[Index] = VariableAddress;

}


inline void AddVariableToVariableTable(variable_table *VariableTable,
                                       uint32 Index, 
                                       asm_string *AsmString, 
                                       uint16 VariableAddress)
{
    AddVariableToVariableTable(VariableTable,
                               Index,
                               AsmString->Contents,
                               AsmString->Length,
                               VariableAddress);
}


variable_table CreatePredefinedVariableTable(void)
{
    variable_table Result;

    Result.VariableNames = 
        (asm_string *)VirtualAlloc(0, 
                                   PREDEFINED_VAR_COUNT*sizeof(asm_string), 
                                   MEM_RESERVE|MEM_COMMIT, 
                                   PAGE_READWRITE);
    Result.VariableAddresses = 
        (uint16 *)VirtualAlloc(0,
                              PREDEFINED_VAR_COUNT*sizeof(uint16),
                              MEM_RESERVE|MEM_COMMIT,
                              PAGE_READWRITE);

    Result.Size = PREDEFINED_VAR_COUNT;

    // NOTE(Marko): IMPORTANT: I didn't null-terminate these strings. This 
    //                         shouldn't be a big problem coz I use asm_string 
    //                         struct which has the length, but just noting 
    //                         this here. 
    AddVariableToVariableTable(&Result, 0, "R0", 2, (uint16)0);
    AddVariableToVariableTable(&Result, 1, "R1", 2, (uint16)1);
    AddVariableToVariableTable(&Result, 2, "R2", 2, (uint16)2);
    AddVariableToVariableTable(&Result, 3, "R3", 2, (uint16)3);
    AddVariableToVariableTable(&Result, 4, "R4", 2, (uint16)4);
    AddVariableToVariableTable(&Result, 5, "R5", 2, (uint16)5);
    AddVariableToVariableTable(&Result, 6, "R6", 2, (uint16)6);
    AddVariableToVariableTable(&Result, 7, "R7", 2, (uint16)7);
    AddVariableToVariableTable(&Result, 8, "R8", 2, (uint16)8);
    AddVariableToVariableTable(&Result, 9, "R9", 2, (uint16)9);
    AddVariableToVariableTable(&Result, 10, "R10", 3, (uint16)10);
    AddVariableToVariableTable(&Result, 11, "R11", 3, (uint16)11);
    AddVariableToVariableTable(&Result, 12, "R12", 3, (uint16)12);
    AddVariableToVariableTable(&Result, 13, "R13", 3, (uint16)13);
    AddVariableToVariableTable(&Result, 14, "R14", 3, (uint16)14);
    AddVariableToVariableTable(&Result, 15, "R15", 3, (uint16)15);

    AddVariableToVariableTable(&Result, 16, "SCREEN", 6, (uint16)16384);
    AddVariableToVariableTable(&Result, 17, "KBD", 3, (uint16)24576);
    AddVariableToVariableTable(&Result, 18, "SP", 2, (uint16)0);
    AddVariableToVariableTable(&Result, 19, "LCL", 3, (uint16)1);
    AddVariableToVariableTable(&Result, 20, "ARG", 3, (uint16)2);
    AddVariableToVariableTable(&Result, 21, "THIS", 4, (uint16)3);
    AddVariableToVariableTable(&Result, 22, "THAT", 4, (uint16)4);

    DEBUGPrintVariableTable(&Result);

    return(Result);                                             
}

internal void InitializeUserDefinedVariableTable(variable_table *VariableTable,
                                                 uint32 Size)
{
    VariableTable->Size = Size;

    VariableTable->VariableNames = 
        (asm_string *)VirtualAlloc(0, 
                                   VariableTable->Size*sizeof(asm_string),
                                   MEM_COMMIT | MEM_RESERVE, 
                                   PAGE_READWRITE);
    VariableTable->VariableAddresses = 
        (uint16 *)VirtualAlloc(0, 
                               VariableTable->Size*sizeof(uint16), 
                               MEM_COMMIT | MEM_RESERVE, 
                               PAGE_READWRITE);
}


internal variable_table CreateLabelTable(uint32 Size)
{
    variable_table Result = {0};
    Result.Size = Size;

    // TODO(Marko): Check that VirtualAlloc() succeeded before moving on! 
    Result.VariableNames =
        (asm_string *)VirtualAlloc(0,
                                   Result.Size*sizeof(asm_string),
                                   MEM_COMMIT | MEM_RELEASE,
                                   PAGE_READWRITE);

    Result.VariableAddresses = 
        (uint16 *)VirtualAlloc(0,
                               Result.Size*sizeof(uint16),
                               MEM_COMMIT | MEM_RELEASE,
                               PAGE_READWRITE);

    return(Result);
} 


internal void FreeVariableTable(variable_table *VariableTable)
{
    // NOTE(Marko): I can't actually free the asm_string structs at the same 
    //              time as freeing the associated strings, or I lose the 
    //              ability to iterate through the asm_string structs! 

    // NOTE(Marko): First free the strings
    for(uint32 i = 0; i < VariableTable->Size; i++)
    {
        VirtualFree(VariableTable->VariableNames[i].Contents, 0, MEM_RELEASE);
    }

    // NOTE(Marko): Then free the strings.
    for(uint32 i = 0; i < VariableTable->Size; i++)
    {
        VirtualFree(&VariableTable->VariableNames[i], 0, MEM_RELEASE);
    }


    // NOTE(Marko): Then release the array of pointers to strings and the 
    //              array of uint16s
    VirtualFree(VariableTable->VariableNames, 0, MEM_RELEASE);
    VirtualFree(VariableTable->VariableAddresses, 0, MEM_RELEASE);
}