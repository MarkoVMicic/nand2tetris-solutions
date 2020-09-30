#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

typedef struct instruction_table
{
    char symbols[8];
    char binary_address_string[16];
} instruction_table;

void insert_instructions_into_dest_table(instruction_table * table);

void insert_instructions_into_comp_table(instruction_table * table);

void insert_instructions_into_jump_table(instruction_table * table);

#endif
