#if !defined(PROCESS_SYMBOLS_H)

// NOTE(Marko): The general use case of the variable table is to iterate 
//              through the VariableNames until we find a match, and only then 
//              do we go to retrieve the address. Thus we organize the 
//              variable table as a Struct of Arrays (SoA), instead of the 
//              usual Array of Structs (AoS), which would pull in the (usually 
//              unused) VariableAddress into memory while iterating through 
//              the VariableNames. 
// TODO(Marko): We can probably turn this into a hash table. Explore this 
//              possibility. 
// TODO(Marko): Do I want to split up the variable_table struct into a 
//              predefined one and user defined one? This is worth considering 
//              because I know ahead of time how big the predefined variable 
//              table is. 
struct variable_table
{
    asm_string *VariableNames;
    uint16 *VariableAddresses;
    uint32 Size;
};


void DEBUGPrintVariableTable(variable_table *VariableTable)
{
    printf("Variable Table Printing:\n\n");
    for(uint32 i = 0; i < VariableTable->Size; i++)
    {
        uint32 StringLength = VariableTable->VariableNames[i].Length;
        char *String = VariableTable->VariableNames[i].Contents;

        for(uint32 j = 0; j < StringLength; j++)
        {
            printf("%c", String[j]);
        }
        printf("\t\t");
        printf("%u", VariableTable->VariableAddresses[i]);
        printf("\n");
    }
}

#define PROCESS_SYMBOLS_H
#endif