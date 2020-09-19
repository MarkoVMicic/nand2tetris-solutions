typedef struct instruction_table_entry
{
    char symbols[8];
    char binary_address_string[16];
} instruction_table_entry;

void insert_instructions_into_dest_table(instruction_table_entry * table);

void insert_instructions_into_comp_table(instruction_table_entry * table);

void insert_instructions_into_jump_table(instruction_table_entry * table);
