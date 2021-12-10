#include "parse_instructions.h"

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


internal void UInt32ToAInstructionString(uint32 Number,
                                         char *BinaryStringBuffer, 
                                         uint32 BinaryStringLength)
{
    // TODO(Marko): Asserts to check that the number is small enough! 
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


internal void UInt32ToAInstructionString(uint32 Number,
                                         asm_string *BinaryAsmString)
{
    UInt32ToAInstructionString(Number, 
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

    for(uint32 ReadAsmIndex = 0; 
        ReadAsmIndex < ReadAsmString->Length; 
        ReadAsmIndex++)
    {
        // NOTE(Marko): This might seem a bit verbose but putting things into 
        //              the asm_string struct makes life simpler
        asm_string MachineCodeLine;
        char MachineCodeLineContents[MACHINE_CODE_LINE_LENGTH];
        MachineCodeLine.Contents = MachineCodeLineContents;
        MachineCodeLine.Length = MACHINE_CODE_LINE_LENGTH;
        ZeroCharInitializeAsmString(&MachineCodeLine);
        switch(ReadAsmString->Contents[ReadAsmIndex])
        {
            case OPEN_BRACKET:
            {
                // NOTE(Marko): This is a label which we have already used, so 
                //              just ignore this line. 
                while(ReadAsmString->Contents[ReadAsmIndex] != NEWLINE)
                {
                    ReadAsmIndex++;
                }

            } break;

            case A_INSTRUCTION_SYMBOL:
            {
                if(IsCharNumber(ReadAsmString->Contents[ReadAsmIndex+1]))
                {
                    asm_string DecimalAddressString;
                    DecimalAddressString.Contents = 
                        &ReadAsmString->Contents[ReadAsmIndex+1];

                    DecimalAddressString.Length = 0;
                    uint32 CurrentCharIndex = 0;
                    while(DecimalAddressString.Contents[CurrentCharIndex] != NEWLINE)
                    {
                        CurrentCharIndex++;
                        DecimalAddressString.Length++;
                    }
                    // TODO(Marko): Can we skip turning the string into a 
                    //              uint32 and go straight to outputting the 
                    //              binary digits?
                    // TODO(Marko): Should this be a uint16?
                    uint32 DecimalAddress = 
                        AsmStringToUInt32(&DecimalAddressString);

                    UInt32ToAInstructionString(DecimalAddress, 
                                               &MachineCodeLine);

                    asm_string CopyLocation;
                    CopyLocation.Contents = &MachineCodeAsmString->Contents[MachineCodeIndex];
                    CopyLocation.Length = MachineCodeAsmString->Length - MachineCodeIndex;
                    CopyAsmString(&MachineCodeLine, &CopyLocation);
                    MachineCodeIndex += MachineCodeLine.Length;
                    
                    // NOTE(Marko) Incrementing by the length of
                    //             DecimalAddressString will push ReadAsmIndex 
                    //             to the char just before the newline. 
                    //             Incrementing it by one more will push it to 
                    //             the newline, after which the for loop will 
                    //             increment it one more time to point at the 
                    //             first char of the next line. 
                    ReadAsmIndex += DecimalAddressString.Length+1;
                }
                else
                {
                    // NOTE(Marko) Use UserDefinedVariableTable to lookup the 
                    //             address and write the binary (stringed) 
                    //             version into MachineCodeLine[].
                    asm_string VariableSymbol;
                    VariableSymbol.Contents = 
                        &ReadAsmString->Contents[ReadAsmIndex+1];
                    VariableSymbol.Length = 0;
                    char *CurrentChar = VariableSymbol.Contents;
                    while(*CurrentChar != NEWLINE)
                    {
                        CurrentChar++;
                        VariableSymbol.Length++;
                    }

                    uint32 FoundIndex = 0;
                    if(WhereInVariableTable(UserDefinedVariableTable,
                                            &VariableSymbol,
                                            &FoundIndex))
                    {
                        uint32 DecimalAddress = UserDefinedVariableTable->VariableAddresses[FoundIndex];
                        UInt32ToAInstructionString(DecimalAddress, 
                                               &MachineCodeLine);

                        asm_string CopyLocation;
                        CopyLocation.Contents = &MachineCodeAsmString->Contents[MachineCodeIndex];
                        CopyLocation.Length = MachineCodeAsmString->Length - MachineCodeIndex;
                        CopyAsmString(&MachineCodeLine, &CopyLocation);
                        MachineCodeIndex += MachineCodeLine.Length;
                    
                    // NOTE(Marko) Incrementing by the length of
                    //             DecimalAddressString will push ReadAsmIndex 
                    //             to the char just before the newline. 
                    //             Incrementing it by one more will push it to 
                    //             the newline, after which the for loop will 
                    //             increment it one more time to point at the 
                    //             first char of the next line. 
                        ReadAsmIndex += VariableSymbol.Length+1;
                    }
                    else
                    {
                        // NOTE(Marko): Variable symbol wasn't found in the 
                        //              UserDefinedVariableTable.
                        InvalidCodePath;
                    }
                }
            } break;

            default:
            {
                // NOTE(Marko): Parsing C-instruction here -- set left-most 
                //              bit to 1. 
                asm_string CInstructionSymbol;
                CInstructionSymbol.Contents = 
                    &ReadAsmString->Contents[ReadAsmIndex];
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
            }
        }
    }
}