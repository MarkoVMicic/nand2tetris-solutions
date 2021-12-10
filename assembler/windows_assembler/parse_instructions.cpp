#include "parse_instructions.h"

internal void ParseInstructions(asm_string *PreprocessedAsmString,
// TODO(Marko): Can make this a lot better using square and multiply algorithm 
//              on the bits than this multiply but it's probably not that 
//              important
inline uint32 Pow(uint32 Base, uint32 Exponent)
{
    uint32 Result = 0;
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



internal uint32 AsmStringToUInt32(asm_string *NumericAsmString)
{
    uint32 Result = 0;
    for(uint32 i = 0; i < NumericAsmString->Length; i++)
    {
        Assert(IsCharNumber(NumericAsmString->Contents[i]));
        Result += (uint32)(NumericAsmString->Contents[i] - '0') * 
                          Pow(10, i);
    }

    return(Result);

}


                                asm_string *MachineCodeAsmString,
                                variable_table *UserDefinedVariableTable)
{
    dest_instruction_table DestTable = CreateDestTable();
    comp_instruction_table CompTable = CreateCompTable();
    jump_instruction_table JumpTable = CreateJumpTable();

    int x = 5;
}