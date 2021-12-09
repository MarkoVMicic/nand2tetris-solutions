#include "parse_instructions.h"

internal void ParseInstructions(asm_string *PreprocessedAsmString,
                                asm_string *MachineCodeAsmString,
                                variable_table *UserDefinedVariableTable)
{
    dest_instruction_table DestTable = CreateDestTable();
    comp_instruction_table CompTable = CreateCompTable();
    jump_instruction_table JumpTable = CreateJumpTable();

    int x = 5;
}