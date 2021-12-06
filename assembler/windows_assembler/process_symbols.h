// NOTE(Marko): The general use case of the variable table is to iterate 
//              through the VariableNames until we find a match, and only then 
//              do we go to retrieve the address. Thus we organize the 
//              variable table as a Struct of Arrays (SoA), instead of the 
//              usual Array of Structs (AoS), which would pull in the (usually 
//              unused) VariableAddress into memory while iterating through 
//              the VariableNames. 
// TODO(Marko): We can probably turn this into a hash table. Explore this 
//              possibility. 
struct variable_table
{
    asm_string *VariableNames;
    uint8 *VariableAddresses;
};

