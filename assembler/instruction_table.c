#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// TODO(Marko): This can be changed to a hash table instead of these arrays.

typedef struct instruction_table
{
    char symbols[8];
    char binary_address_string[16];
} instruction_table;


void insert_instructions_into_dest_table(instruction_table * table)
{
    strcpy(table[0].symbols, "\0");
    strcpy(table[0].binary_address_string, "000"); 

    strcpy(table[1].symbols, "M"); 
    strcpy(table[1].binary_address_string, "001"); 

    strcpy(table[2].symbols, "D"); 
    strcpy(table[2].binary_address_string, "010"); 

    strcpy(table[3].symbols, "MD");
    strcpy(table[3].binary_address_string, "011"); 

    strcpy(table[4].symbols, "A");
    strcpy(table[4].binary_address_string, "100"); 

    strcpy(table[5].symbols, "AM");
    strcpy(table[5].binary_address_string, "101"); 

    strcpy(table[6].symbols, "AD");
    strcpy(table[6].binary_address_string, "000"); 

    strcpy(table[7].symbols, "AMD");
    strcpy(table[7].binary_address_string, "000");   
}


void insert_instructions_into_comp_table(instruction_table * table)
{
   strcpy(table[0].symbols, "0");
   strcpy(table[0].binary_address_string, "0101010");
   
   strcpy(table[1].symbols, "1");
   strcpy(table[1].binary_address_string, "0111111");
   
   strcpy(table[2].symbols, "-1");
   strcpy(table[2].binary_address_string, "0111010");
   
   strcpy(table[3].symbols, "D");
   strcpy(table[3].binary_address_string, "0001100");
   
   strcpy(table[4].symbols, "A");
   strcpy(table[4].binary_address_string, "0110000");
   
   strcpy(table[5].symbols, "!D");
   strcpy(table[5].binary_address_string, "0001101");
   
   strcpy(table[6].symbols, "!A");
   strcpy(table[6].binary_address_string, "0110001");
   
   strcpy(table[7].symbols, "-D");
   strcpy(table[7].binary_address_string, "0001111");
   
   strcpy(table[8].symbols, "-A");
   strcpy(table[8].binary_address_string, "0110011");
   
   strcpy(table[9].symbols, "D+1");
   strcpy(table[9].binary_address_string, "0011111");
   
   strcpy(table[10].symbols, "A+1");
   strcpy(table[10].binary_address_string, "0110010");
   
   strcpy(table[11].symbols, "D-1");
   strcpy(table[11].binary_address_string, "0001110");
   
   strcpy(table[12].symbols, "A-1");
   strcpy(table[12].binary_address_string, "0110010");
   
   strcpy(table[13].symbols, "D+A");
   strcpy(table[13].binary_address_string, "0000010");
   
   strcpy(table[14].symbols, "D-A");
   strcpy(table[14].binary_address_string, "0010011");
   
   strcpy(table[15].symbols, "A-D");
   strcpy(table[15].binary_address_string, "0000111");
   
   strcpy(table[16].symbols, "D&A");
   strcpy(table[16].binary_address_string, "0000000");
   
   strcpy(table[17].symbols, "D|A");
   strcpy(table[17].binary_address_string, "0010101");
   
   strcpy(table[18].symbols, "M");
   strcpy(table[18].binary_address_string, "1110000");
   
   strcpy(table[19].symbols, "!M");
   strcpy(table[19].binary_address_string, "1110001");

   strcpy(table[20].symbols, "-M");
   strcpy(table[20].binary_address_string, "1110011");
   
   strcpy(table[21].symbols, "M+1");
   strcpy(table[21].binary_address_string, "1110111");
   
   strcpy(table[22].symbols, "M-1");
   strcpy(table[22].binary_address_string, "1110010");
   
   strcpy(table[23].symbols, "D+M");
   strcpy(table[23].binary_address_string, "1000010");
   
   strcpy(table[24].symbols, "D-M");
   strcpy(table[24].binary_address_string, "1010011");
   
   strcpy(table[25].symbols, "M-D");
   strcpy(table[25].binary_address_string, "1000111");
   
   strcpy(table[26].symbols, "D&M");
   strcpy(table[26].binary_address_string, "1000000");
   
   strcpy(table[27].symbols, "D|M");
   strcpy(table[27].binary_address_string, "1010101");

}

void insert_instructions_into_jump_table(instruction_table * table)
{
    strcpy(table[0].symbols, "\0");
    strcpy(table[0].binary_address_string, "000"); 

    strcpy(table[1].symbols, "JGT"); 
    strcpy(table[1].binary_address_string, "001"); 

    strcpy(table[2].symbols, "JEQ"); 
    strcpy(table[2].binary_address_string, "010"); 

    strcpy(table[3].symbols, "JGE");
    strcpy(table[3].binary_address_string, "011"); 

    strcpy(table[4].symbols, "JLT");
    strcpy(table[4].binary_address_string, "100"); 

    strcpy(table[5].symbols, "JNE");
    strcpy(table[5].binary_address_string, "101"); 

    strcpy(table[6].symbols, "JLE");
    strcpy(table[6].binary_address_string, "000"); 

    strcpy(table[7].symbols, "JMP");
    strcpy(table[7].binary_address_string, "000");   
}