#include "parse_instructions.h"

// TODO(Marko): Can make this a lot better using square and multiply algorithm 
//              on the bits than this multiply but it's probably not that 
//              important
inline uint32 Pow(uint32 Base, uint32 Exponent)
{
    uint32 Result = 1;
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
                          Pow(10, (NumericAsmString->Length-1) - i);
    }

    return(Result);

}


internal void UInt32ToBinaryString(uint32 Number,
                                   char *BinaryStringBuffer, 
                                   uint32 BinaryStringLength)
{
    Assert(Number < 0xFFFF);
    uint32 CharsRemaining = BinaryStringLength;
    // NOTE(Marko): For A-instructions, set the msb to 0. 
    BinaryStringBuffer[0] ='0';
    CharsRemaining--;

    // NOTE(Marko): Final char in the line is always newline.
    BinaryStringBuffer[BinaryStringLength-1] = NEWLINE;
    CharsRemaining--;

    // NOTE(Marko): Shortcut for 0
    if(Number == 0)
    {
        for(uint32 BinaryStringIndex = 1; 
            BinaryStringIndex < CharsRemaining; BinaryStringIndex++)
        {
            BinaryStringBuffer[BinaryStringIndex] = '0';
        }
    }
    else
    {
        // NOTE(Marko) Insert the bits from left to right by using mod. 
        uint32 BinaryStringIndex = BinaryStringLength-2;
        while(Number > 0)
        {
            BinaryStringBuffer[BinaryStringIndex] = (char)((Number%2) + '0');
            Number = Number/2;
            BinaryStringIndex--;
        }
    }
}


internal void UInt32ToBinaryAsmString(uint32 Number,
                                         asm_string *BinaryAsmString)
{
    UInt32ToBinaryString(Number, 
                         BinaryAsmString->Contents, 
                         BinaryAsmString->Length);
}



internal void ZeroCharInitializeString(char *String, 
                                       uint32 StringLength)
{
    for(uint32 StringIndex = 0; StringIndex < StringLength; StringIndex++)
    {
        String[StringIndex] = '0';
    }
}


internal void ZeroCharInitializeAsmString(asm_string *AsmString)
{
    ZeroCharInitializeString(AsmString->Contents, AsmString->Length);
}


internal bool32 WhereInAsmString(asm_string *AsmString, 
                                 char Char, 
                                 uint32 *FoundIndex)
{
    bool32 Result = false;
    for(uint32 i = 0; i < AsmString->Length; i++)
    {
        if(AsmString->Contents[i] == Char)
        {
            Result = true;
            *FoundIndex = i;
            break;
        }
    }
    return(Result);
}


internal void ParseInstructions(asm_string *ReadAsmString,
                                asm_string *MachineCodeAsmString,
                                variable_table *UserDefinedVariableTable)
{
    dest_instruction_table DestTable = CreateDestTable();
    comp_instruction_table CompTable = CreateCompTable();
    jump_instruction_table JumpTable = CreateJumpTable();

    uint32 MachineCodeIndex = 0;
    uint32 LineCount = 0;

    for(uint32 ReadIndex = 0; 
        ReadIndex < ReadAsmString->Length; 
        ReadIndex++)
    {
        // NOTE(Marko): This might seem a bit verbose but putting things into 
        //              the asm_string struct makes life simpler
        asm_string MachineCodeLine;
        char MachineCodeLineContents[MACHINE_CODE_LINE_LENGTH];
        MachineCodeLine.Contents = MachineCodeLineContents;
        MachineCodeLine.Length = MACHINE_CODE_LINE_LENGTH;
        ZeroCharInitializeAsmString(&MachineCodeLine);

        bool32 ShouldCopy = true;

        switch(ReadAsmString->Contents[ReadIndex])
        {
            case OPEN_BRACKET:
            {
                // NOTE(Marko): This is a label which we have already used, so 
                //              just ignore this line. 
                ShouldCopy = false;
                while(ReadAsmString->Contents[ReadIndex] != NEWLINE)
                {
                    ReadIndex++;
                }

            } break;

            case A_INSTRUCTION_SYMBOL:
            {
                asm_string AInstructionSymbol;
                AInstructionSymbol.Contents = 
                    &ReadAsmString->Contents[ReadIndex + 1];
                AInstructionSymbol.Length = 0;
                char *CurrentChar = AInstructionSymbol.Contents;
                while(*CurrentChar != NEWLINE)
                {
                    AInstructionSymbol.Length++;
                    CurrentChar++;
                }

                // NOTE(Marko): Check if first character is numeric or not
                uint32 DecimalAddress = 0;
                if(!IsCharNumber(AInstructionSymbol.Contents[0]))
                {
                    // NOTE(Marko): If it's not numeric, lookup the decimal 
                    //              address in the UserDefinedVariableTable
                    uint32 FoundIndex = 0;
                    if(WhereInVariableTable(UserDefinedVariableTable,
                                            &AInstructionSymbol,
                                            &FoundIndex))
                    {
                        DecimalAddress = UserDefinedVariableTable->VariableAddresses[FoundIndex];
                    }
                    else
                    {
                        // NOTE(Marko): Somehow we have a non-numeric 
                        //              AInstructionSymbol that isn't in our 
                        //              UserDefinedVariableTable, which is an 
                        //              error.
                        // TODO(Marko): Error handling
                        InvalidCodePath;
                    }
                }
                else
                {
                    // NOTE(Marko): If it is numeric, then convert the string 
                    //              into a uint32.
                    DecimalAddress = AsmStringToUInt32(&AInstructionSymbol);
                }

                // NOTE(Marko): Convert uint32 into "binary string"
                UInt32ToBinaryAsmString(DecimalAddress, 
                                        &MachineCodeLine);

                // NOTE(Marko) Incrementing by the length of
                //             AInstructionSymbol will push ReadIndex 
                //             to the char just before the newline. 
                //             Incrementing it by one more will push it to 
                //             the newline, after which the for loop will 
                //             increment it one more time to point at the 
                //             first char of the next line. 
                ReadIndex += AInstructionSymbol.Length+1;

            } break;

            default:
            {
                // NOTE(Marko): Parsing C-instruction here -- set left-most 
                //              bit to 1. 
                asm_string CInstructionSymbol;
                CInstructionSymbol.Contents = 
                    &ReadAsmString->Contents[ReadIndex];
                CInstructionSymbol.Length = 0;
                char *CurrentChar = CInstructionSymbol.Contents;
                while(*CurrentChar != NEWLINE)
                {
                    CurrentChar++;
                    CInstructionSymbol.Length++;
                }
                // NOTE(Marko): Dest portion of C-instruction. Starts at 
                //              beginning of line and ends at equals sign. If 
                //              there is no equals sign, then its null and we 
                //              simply write "000" for that portion of the 
                //              MachineCodeLine
                asm_string DestBinaryAsmString;
                char DestBinaryAsmStringContents[DEST_BINARY_PORTION_LENGTH];
                DestBinaryAsmString.Contents = DestBinaryAsmStringContents;
                DestBinaryAsmString.Length = DEST_BINARY_PORTION_LENGTH;
                ZeroCharInitializeAsmString(&DestBinaryAsmString); 
                uint32 EqualsSignIndex = 0;
                // NOTE(Marko): This if statement sets EqualsSignIndex. 
                //              EqualsSignIndex cannot actually be 0, since a 
                //              C-instruction cannot start with an equals 
                //              sign. So if its 0, it means that there is no 
                //              Equals Sign and there is no Dest portion. We 
                //              need to remember this fact later when deciding 
                //              where the Comp portion begins and ends. 
                if(WhereInAsmString(&CInstructionSymbol, 
                                    EQUALS_SIGN, 
                                    &EqualsSignIndex))
                {
                    // NOTE(Marko): Seek from beginning of C-instruction to 
                    //              Equals sign
                    asm_string DestSymbol;
                    DestSymbol.Contents = CInstructionSymbol.Contents;
                    // NOTE(Marko): Instead of just scanning through the 
                    //              CInstruction Symbol again, we just use the 
                    //              position of the EqualsSign (i.e. 
                    //              EqualsSignIndex)
                    DestSymbol.Length = EqualsSignIndex;
                    uint32 DestTableFoundIndex = 0;
                    if(WhereInDestTable(&DestTable, 
                                        &DestSymbol, 
                                        &DestTableFoundIndex))
                    {
                        CopyAsmString(&DestTable.MachineCodeTranslations[DestTableFoundIndex], &DestBinaryAsmString);
                    }
                    else
                    {
                        // NOTE(Marko): The DestSymbol wasn't found in the 
                        //              DestTable. 
                        InvalidCodePath;
                    }
                }
                else
                {
                    // NOTE(Marko): We already zero-char-initialized the 
                    //              DestBinaryAsmString so we don't need to do 
                    //              anything here, since a null DEST 
                    //              corresponds to "000"
                }

                // NOTE(Marko): Comp portion of C-instruction. Starts at 
                //              beginning of equals sign (if there is one) and 
                //              ends at semicolon (if there is one). There is 
                //              no legal C-instruction that doesn't have a 
                //              Comp portion.
                asm_string CompBinaryAsmString;
                char CompBinaryAsmStringContents[COMP_BINARY_PORTION_LENGTH];
                CompBinaryAsmString.Contents = CompBinaryAsmStringContents;
                CompBinaryAsmString.Length = COMP_BINARY_PORTION_LENGTH;
                // TODO(Marko): Evaluate if we actually want to spend the time
                //              doing this particular brand of zero 
                //              initialization for the comp binary string, 
                //              since there is no NULL string. 
                ZeroCharInitializeAsmString(&CompBinaryAsmString);

                uint32 CompBeginIndex = 0; 
                if(EqualsSignIndex != 0)
                {
                    CompBeginIndex = EqualsSignIndex + 1;
                }

                uint32 CompEndIndex = 0;
                uint32 SemiColonIndex = 0;
                // NOTE(Marko): This if statement sets SemiColonIndex. 
                //              SemiColonIndex cannot actually be 0 -- 
                //              C-instruction cannot start with a semicolon 
                if(WhereInAsmString(&CInstructionSymbol, 
                                    SEMICOLON, 
                                    &SemiColonIndex))
                {
                    CompEndIndex = SemiColonIndex - 1;
                }
                else
                {
                    CompEndIndex = CInstructionSymbol.Length - 1;
                }

                asm_string CompSymbol;
                CompSymbol.Contents = &CInstructionSymbol.Contents[CompBeginIndex];
                CompSymbol.Length = CompEndIndex - CompBeginIndex + 1;

                uint32 CompTableFoundIndex = 0;
                if(WhereInCompTable(&CompTable, 
                                    &CompSymbol, 
                                    &CompTableFoundIndex))
                {
                    CopyAsmString(&CompTable.MachineCodeTranslations[CompTableFoundIndex], &CompBinaryAsmString);
                } 
                else
                {
                    // NOTE(Marko): Comp symbol not found in comp table. 
                    InvalidCodePath;
                }

                // NOTE(Marko): Jump portion of C-instruction. Starts at the 
                //              beginning of the semicolon (if there is one) 
                //              and ends at the newline. If there is no 
                //              semicolon, then it's a null instruction. 
                asm_string JumpBinaryAsmString;
                char JumpBinaryAsmStringContents[JUMP_BINARY_PORTION_LENGTH];
                JumpBinaryAsmString.Contents = JumpBinaryAsmStringContents;
                JumpBinaryAsmString.Length = JUMP_BINARY_PORTION_LENGTH;
                ZeroCharInitializeAsmString(&JumpBinaryAsmString);
                // NOTE(Marko): We don't have to process this if there is no 
                //              semicolon, just stick with the "null" string 
                //              (i.e. "000")
                if(SemiColonIndex != 0)
                {
                    uint32 JumpBeginIndex = SemiColonIndex + 1;
                    // NOTE(Marko): Don't need a JumpEndIndex since that's 
                    //              just the end of the CInstructionSymbol
                    asm_string JumpSymbol;
                    JumpSymbol.Contents = &CInstructionSymbol.Contents[JumpBeginIndex];
                    // NOTE(Marko): JumpSymbol should always have length 3! 
                    //              (Look at the table in the header file to 
                    //              see why)
                    JumpSymbol.Length = CInstructionSymbol.Length - JumpBeginIndex;

                    uint32 JumpTableFoundIndex = 0;
                    if(WhereInJumpTable(&JumpTable, 
                                        &JumpSymbol, 
                                        &JumpTableFoundIndex))
                    {
                        CopyAsmString(&JumpTable.MachineCodeTranslations[JumpTableFoundIndex], &JumpBinaryAsmString);
                    }
                    else
                    {
                        // NOTE(Marko): Jump symbol not found in Jump Table.
                        InvalidCodePath;
                    }
                }

                // TODO(Marko): We can get CompBinaryAsmString, 
                //              DestBinaryAsmString, and JumpBinaryAsmString 
                //              to point directly to MachineCodeLine[] and 
                //              just write into it immediately instead of 
                //              composing everything at the end here. (This 
                //              will also allow us to not need to use //              CopyString and instead use CopyAsmString)
                // NOTE(Marko): Assemble the MachineCodeLine using the 3 
                //              binary strings. 
                // NOTE(Marko): C-Instruction:
                //         
                //      Index       0 1 2 3 4  5  6  7  8  9  10 11 12 13 14 15
                //      Data        1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
                //
                //      CompBinaryAsmString.Contents == [a,c1,c2,c3,c4,c5,c6]
                //      DestBinaryAsmString.Contents == [d1, d2, d3]
                //      JumpBinaryAsmString.Contents == [j1, j2, j3]

                // NOTE(Marko): First three digits are always 1 in a 
                //              c-instruction. 
                MachineCodeLine.Contents[0] = '1';
                MachineCodeLine.Contents[1] = '1';
                MachineCodeLine.Contents[2] = '1';

                CopyString(CompBinaryAsmString.Contents, 
                           CompBinaryAsmString.Length,
                           &MachineCodeLine.Contents[3],
                           MachineCodeLine.Length - 3);
                CopyString(DestBinaryAsmString.Contents, 
                           DestBinaryAsmString.Length,
                           &MachineCodeLine.Contents[10],
                           MachineCodeLine.Length - 10);
                CopyString(JumpBinaryAsmString.Contents, 
                           JumpBinaryAsmString.Length,
                           &MachineCodeLine.Contents[13],
                           MachineCodeLine.Length - 13);

                ReadIndex += CInstructionSymbol.Length;
            }
        }

        if(ShouldCopy)
        {
            // NOTE(Marko): Copy the MachineCodeLine into the 
            //              MachineCodeAsmString.
            // NOTE(Marko): Remember to end the line with a newline
            MachineCodeLine.Contents[MACHINE_CODE_LINE_LENGTH-1] = '\n';
            CopyString(MachineCodeLine.Contents,
                       MachineCodeLine.Length,
                       &MachineCodeAsmString->Contents[MachineCodeIndex],
                       MachineCodeAsmString->Length - MachineCodeIndex);

            MachineCodeIndex += MachineCodeLine.Length;
            LineCount++;
        }
    }
}