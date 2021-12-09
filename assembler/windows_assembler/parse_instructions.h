#if !defined(PARSE_INSTRUCTIONS_H)

// NOTE(Marko): 16 bit instructions + a newline char at the end.
#define MACHINE_CODE_LINE_LENGTH 17
// NOTE(Marko): DEST_INSTRUCTION_COUNT does not include the null instruction.
#define DEST_INSTRUCTION_COUNT 7
#define COMP_INSTRUCTION_COUNT 28
// NOTE(Marko): JUMP_INSTRUCTION_COUNT also does not include null instruction.
#define JUMP_INSTRUCTION_COUNT 7

struct dest_instruction_table
{
    asm_string Tokens[DEST_INSTRUCTION_COUNT];
    asm_string MachineCodeTranslations[DEST_INSTRUCTION_COUNT];
    uint32 Size;
};

struct comp_instruction_table
{
    asm_string Tokens[COMP_INSTRUCTION_COUNT];
    asm_string MachineCodeTranslations[COMP_INSTRUCTION_COUNT];
};

struct jump_instruction_table
{
    asm_string Tokens[JUMP_INSTRUCTION_COUNT];
    asm_string MachineCodeTranslations[JUMP_INSTRUCTION_COUNT];
};


internal comp_instruction_table CreateCompTable(void)
{
    /*  NOTE(Marko):

            COMP
            ------------------
            0          0101010
            1          0111111
            -1         0111010
            D          0001100
            A          0110000
            !D         0001101
            !A         0110001
            -D         0001111
            -A         0110011
            D+1        0011111
            A+1        0110111
            D-1        0001110
            A-1        0110010
            D+A        0000010
            D-A        0010011
            A-D        0000111
            D&A        0000000
            D|A        0010101
            M          1110000
            !M         1110001
            -M         1110011
            M+1        1110111
            M-1        1110010
            D+M        1000010
            D-M        1010011
            M-D        1000111
            D&M        1000000
            D|M        1010101

    */
    comp_instruction_table Result = {0};
    Result.Size = COMP_INSTRUCTION_COUNT;

    Result.Tokens[0].Contents = "0";
    Result.Tokens[0].Length = 1;

    Result.Tokens[1].Contents = "1";
    Result.Tokens[1].Length = 1;

    Result.Tokens[2].Contents = "-1";
    Result.Tokens[2].Length = 2;  

    Result.Tokens[3].Contents = "D";
    Result.Tokens[3].Length = 1;  

    Result.Tokens[4].Contents = "A";
    Result.Tokens[4].Length = 1;  

    Result.Tokens[5].Contents = "!D";
    Result.Tokens[5].Length = 2;  

    Result.Tokens[6].Contents = "!A";
    Result.Tokens[6].Length = 2;  

    Result.Tokens[7].Contents = "-D";
    Result.Tokens[7].Length = 2;  

    Result.Tokens[8].Contents = "-A";
    Result.Tokens[8].Length = 2;  

    Result.Tokens[9].Contents = "D+1";
    Result.Tokens[9].Length = 3;  

    Result.Tokens[10].Contents = "A+1";
    Result.Tokens[10].Length = 3;   

    Result.Tokens[11].Contents = "D-1";
    Result.Tokens[11].Length = 3;   

    Result.Tokens[12].Contents = "A-1";
    Result.Tokens[12].Length = 3;   

    Result.Tokens[13].Contents = "D+A";
    Result.Tokens[13].Length = 3;   

    Result.Tokens[14].Contents = "D-A";
    Result.Tokens[14].Length = 3;  

    Result.Tokens[15].Contents = "A-D";
    Result.Tokens[15].Length = 3;  

    Result.Tokens[16].Contents = "D&A";
    Result.Tokens[16].Length = 3;  

    Result.Tokens[17].Contents = "D|A";
    Result.Tokens[17].Length = 3;  

    Result.Tokens[18].Contents = "M";
    Result.Tokens[18].Length = 1;  

    Result.Tokens[19].Contents = "!M";
    Result.Tokens[19].Length = 2;  

    Result.Tokens[20].Contents = "-M";
    Result.Tokens[20].Length = 2; 

    Result.Tokens[21].Contents = "M+1";
    Result.Tokens[21].Length = 3;  

    Result.Tokens[22].Contents = "M-1";
    Result.Tokens[22].Length = 3;   

    Result.Tokens[23].Contents = "D+M";
    Result.Tokens[23].Length = 3;   

    Result.Tokens[24].Contents = "D-M";
    Result.Tokens[24].Length = 3;   

    Result.Tokens[25].Contents = "M-D";
    Result.Tokens[25].Length = 3;   

    Result.Tokens[26].Contents = "D&M";
    Result.Tokens[26].Length = 3;   

    Result.Tokens[27].Contents = "D|M";
    Result.Tokens[27].Length = 3;  


    Result.MachineCodeTranslations[0].Contents = "0101010";
    Result.MachineCodeTranslations[0].Length = 7;

    Result.MachineCodeTranslations[1].Contents = "0111111";
    Result.MachineCodeTranslations[1].Length = 7;

    Result.MachineCodeTranslations[2].Contents = "0111010";
    Result.MachineCodeTranslations[2].Length = 7;

    Result.MachineCodeTranslations[3].Contents = "0001100";
    Result.MachineCodeTranslations[3].Length = 7;

    Result.MachineCodeTranslations[4].Contents = "0110000";
    Result.MachineCodeTranslations[4].Length = 7;

    Result.MachineCodeTranslations[5].Contents = "0001101";
    Result.MachineCodeTranslations[5].Length = 7;

    Result.MachineCodeTranslations[6].Contents = "0110001";
    Result.MachineCodeTranslations[6].Length = 7;

    Result.MachineCodeTranslations[7].Contents = "0001111";
    Result.MachineCodeTranslations[7].Length = 7;

    Result.MachineCodeTranslations[8].Contents = "0110011";
    Result.MachineCodeTranslations[8].Length = 7;

    Result.MachineCodeTranslations[9].Contents = "0011111";
    Result.MachineCodeTranslations[9].Length = 7;

    Result.MachineCodeTranslations[10].Contents = "0110111";
    Result.MachineCodeTranslations[10].Length = 7;

    Result.MachineCodeTranslations[11].Contents = "0001110";
    Result.MachineCodeTranslations[11].Length = 7;

    Result.MachineCodeTranslations[12].Contents = "0110010";
    Result.MachineCodeTranslations[12].Length = 7;

    Result.MachineCodeTranslations[13].Contents = "0000010";
    Result.MachineCodeTranslations[13].Length = 7;

    Result.MachineCodeTranslations[14].Contents = "0010011";
    Result.MachineCodeTranslations[14].Length = 7;

    Result.MachineCodeTranslations[15].Contents = "0000111";
    Result.MachineCodeTranslations[15].Length = 7;

    Result.MachineCodeTranslations[16].Contents = "0000000";
    Result.MachineCodeTranslations[16].Length = 7;

    Result.MachineCodeTranslations[17].Contents = "0010101";
    Result.MachineCodeTranslations[17].Length = 7;

    Result.MachineCodeTranslations[18].Contents = "1110000";
    Result.MachineCodeTranslations[18].Length = 7;

    Result.MachineCodeTranslations[19].Contents = "1110001";
    Result.MachineCodeTranslations[19].Length = 7;

    Result.MachineCodeTranslations[20].Contents = "1110011";
    Result.MachineCodeTranslations[20].Length = 7;

    Result.MachineCodeTranslations[21].Contents = "1110111";
    Result.MachineCodeTranslations[21].Length = 7;

    Result.MachineCodeTranslations[22].Contents = "1110010";
    Result.MachineCodeTranslations[22].Length = 7;

    Result.MachineCodeTranslations[23].Contents = "1000010";
    Result.MachineCodeTranslations[23].Length = 7;

    Result.MachineCodeTranslations[24].Contents = "1010011";
    Result.MachineCodeTranslations[24].Length = 7;

    Result.MachineCodeTranslations[25].Contents = "1000111";
    Result.MachineCodeTranslations[25].Length = 7;

    Result.MachineCodeTranslations[26].Contents = "1000000";
    Result.MachineCodeTranslations[26].Length = 7;

    Result.MachineCodeTranslations[27].Contents = "1010101";
    Result.MachineCodeTranslations[27].Length = 7;

    return(Result);
     
}


internal jump_instruction_table CreateJumpTable(void)
{
    /* NOTE(Marko): NULL is not needed. Instead we need to check if there's a 
                    semicolon

            JUMP
            -----------------
            NULL    000
            JGT     001
            JEQ     010
            JGE     011
            JLT     100
            JNE     101
            JLE     110
            JMP     111

    */
    jump_instruction_table Result = {0};
    Result.Size = JUMP_INSTRUCTION_COUNT;

    Result.Tokens[0].Contents = "JGT";     
    Result.Tokens[0].Length = 3;

    Result.Tokens[1].Contents = "JEQ";    
    Result.Tokens[1].Length = 3;

    Result.Tokens[2].Contents = "JGE";    
    Result.Tokens[2].Length = 3;

    Result.Tokens[3].Contents = "JLT";    
    Result.Tokens[3].Length = 3;

    Result.Tokens[4].Contents = "JNE";
    Result.Tokens[4].Length = 3;

    Result.Tokens[5].Contents = "JLE";
    Result.Tokens[5].Length = 3;

    Result.Tokens[6].Contents = "JMP";
    Result.Tokens[6].Length = 3;


    Result.MachineCodeTranslations[0].Contents = "001";
    Result.MachineCodeTranslations[0].Length = 3;
    Result.MachineCodeTranslations[1].Contents = "010";
    Result.MachineCodeTranslations[1].Length = 3;
    Result.MachineCodeTranslations[2].Contents = "011";
    Result.MachineCodeTranslations[2].Length = 3;
    Result.MachineCodeTranslations[3].Contents = "100";
    Result.MachineCodeTranslations[3].Length = 3;
    Result.MachineCodeTranslations[4].Contents = "101";
    Result.MachineCodeTranslations[4].Length = 3;
    Result.MachineCodeTranslations[5].Contents = "110";
    Result.MachineCodeTranslations[5].Length = 3;
    Result.MachineCodeTranslations[6].Contents = "111";
    Result.MachineCodeTranslations[6].Length = 3;

    return(Result);
   
}


internal dest_instruction_table CreateDestTable(void)
{
    /* NOTE(Marko): NULL is not needed. Instead we need to check if there's an 
                    equals sign.

            DEST
            ---------------
            NULL    000
            M       001
            D       010
            MD      011
            A       100
            AM      101
            AD      110
            AMD     111
    */
    dest_instruction_table Result = {0};
    Result.Size = DEST_INSTRUCTION_COUNT;

    Result.Tokens[0].Contents = "M";
    Result.Tokens[0].Length = 1;

    Result.Tokens[1].Contents = "D";
    Result.Tokens[1].Length = 1;

    Result.Tokens[2].Contents = "MD";
    Result.Tokens[2].Length = 2;

    Result.Tokens[3].Contents = "A";
    Result.Tokens[3].Length = 1;

    Result.Tokens[4].Contents = "AM";
    Result.Tokens[4].Length = 2;

    Result.Tokens[5].Contents = "AD";
    Result.Tokens[5].Length = 2;

    Result.Tokens[6].Contents = "AMD";
    Result.Tokens[6].Length = 3;

    Result.MachineCodeTranslations[0].Contents = "001";
    Result.MachineCodeTranslations[0].Length = 3;

    Result.MachineCodeTranslations[1].Contents = "010";
    Result.MachineCodeTranslations[1].Length = 3;

    Result.MachineCodeTranslations[2].Contents = "011";
    Result.MachineCodeTranslations[2].Length = 3;

    Result.MachineCodeTranslations[3].Contents = "100";
    Result.MachineCodeTranslations[3].Length = 3;

    Result.MachineCodeTranslations[4].Contents = "101";
    Result.MachineCodeTranslations[4].Length = 3;

    Result.MachineCodeTranslations[5].Contents = "110";
    Result.MachineCodeTranslations[5].Length = 3;

    Result.MachineCodeTranslations[6].Contents = "111";
    Result.MachineCodeTranslations[6].Length = 3;

    return(Result);

}

#define PARSE_INSTRUCTIONS_H
#endif