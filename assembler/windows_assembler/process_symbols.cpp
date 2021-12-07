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

// TODO(Marko): This probably belongs in a header file?
void CopyString(char *SourceString, 
                uint32 SourceStringLength,
                char *DestString, 
                uint32 DestStringLength)
{
    Assert(DestStringLength >= SourceStringLength);
    
    for(uint32 i = 0; i < DestStringLength; i++)
    {
        DestString[i] = SourceString[i];
    }
}

inline void AddVariableToVariableTable(variable_table *VariableTable,
                                uint32 Index, 
                                char *VariableName, 
                                uint32 VariableNameLength, 
                                uint8 VariableAddress)
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

    // NOTE(Marko): IMPORTANT: I didn't null-terminate these strings. This shouldn't be a big problem coz I use asm_string struct which has the length, but just noting this here. 
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


    return(Result);                                             
}

internal void ProcessSymbols(asm_string *AsmString)
{
    variable_table PredefinedVariableTable = CreatePredefinedVariableTable();
}