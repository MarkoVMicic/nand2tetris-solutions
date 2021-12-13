#if !defined(VARIABLE_TABLE_H)

#define PREDEFINED_VAR_COUNT 23

// NOTE(Marko): The general use case of the variable table is to iterate 
//              through the VariableNames until we find a match, and only then 
//              do we go to retrieve the address. Thus we organize the 
//              variable table as a Struct of Arrays (SoA), instead of the 
//              usual Array of Structs (AoS), which would pull in the (usually 
//              unused) VariableAddress into memory while iterating through 
//              the VariableNames. 
// TODO(Marko): We can probably turn this into a hash table. Explore this 
//              possibility. 
// TODO(Marko): Explore splitting up variable_table struct into 
//              predefined_variable_table and user_defined_variable_table 
//              structs, with the difference being that 
//              predefined_variable_table is known ahead of time and so can 
//              have all sizes stated explicitly using #defines etc
struct variable_table
{
    asm_string *VariableNames;
    uint16 *VariableAddresses;
    uint32 Size;
};


#define VARIABLE_TABLE_H
#endif