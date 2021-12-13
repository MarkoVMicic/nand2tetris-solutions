#include "preprocess.h"


// NOTE(Marko): I think the compiler will make this fast? It's the simplest 
//              thing I can think of xD. Since it's a uint16 it can't get 
//              bigger than 2^16-1 = 65535 which is 5 digits. 
uint32 NumberOfDigits(uint16 Number)
{
    Assert(Number >= 0);
    uint32 Result = 0;

    if (Number < 10) 
    {
        Result = 1;
        return(Result);
    }
    if (Number < 100) 
    {
        Result = 2;
        return(Result);
    }
    if (Number < 1000) 
    {
        Result = 3;
        return(Result);
    }
    if (Number < 10000) 
    {
        Result = 4;
        return(Result);
    }
    if (Number < 100000) 
    {
        Result = 5;
        return(Result);
    }
    return(Result);
}


asm_string UInt16ToAsmString(uint16 UInt16)
{
    asm_string Result;
    uint32 DigitCount = NumberOfDigits(UInt16);

    Result.Length = DigitCount;
    Result.Contents = (char *)VirtualAlloc(0, 
                                           (DigitCount+1)*sizeof(char), 
                                           MEM_RESERVE | MEM_COMMIT, 
                                           PAGE_READWRITE);

    _snprintf_s(Result.Contents, Result.Length+1, Result.Length, "%u", UInt16);

    return(Result);
}


internal void PreprocessAsmString(asm_string *ReadAsmString, 
                                  variable_table *PredefinedVariableTable,
                                  variable_table *UserDefinedVariableTable,
                                  uint32 *LineCount)
{
    // NOTE(Marko): Here, we create a block of memory of identical 
    //              size to ReadAsmString: one for ReadAsmString and one for 
    //              WriteAsmString. 
    //              For each pass through the string of assembly 
    //              instructions, we will selectively copy over characters 
    //              from ReadAsmString to WriteAsmString. At the end of each 
    //              pass, we will swap them (i.e. WriteAsmString becomes 
    //              ReadAsmString, and vice versa). In this way, we can 
    //              easily manage the editing of the strings without 
    //              having to worry about multiple allocations and frees. 
    //              Indeed the only thing we need to allocate on the heap 
    //              are two strings, of equal size. Since we remove all 
    //              the comments and whitespace, it's unlikely that we 
    //              will ever need more space. And since asm_string keeps 
    //              track of length, it's easy to find out when we've come 
    //              to the end of any of those strings.
    //              Consider that the WriteAsmString is always just a block 
    //              of memory to write into, and the ReadAsmString is the 
    //              block of memory that we read from. 
    // NOTE(Marko): I'm grabbing a pointer here to keep things syntatically 
    //              symmetric with the ReadAsmString (which is also a pointer)
    asm_string *WriteAsmString = &InitializeAsmString(ReadAsmString->Length);


    // TODO(Marko): Considering scoping each pass. 
 
    //
    // NOTE(Marko): First pass    
    //

    uint32 WriteIndex = 0;
    uint32 RemovedCharsCount = 0;
    uint32 UserDefinedVariableCount = 0;
    uint32 LabelCount = 0;
    for(uint32 ReadIndex = 0; ReadIndex < ReadAsmString->Length; ReadIndex++)
    {
        // TODO(Marko): This is a useful place to start recording errors. What 
        //              would we need to do that? An error struct maybe, which 
        //              gets passed into here, and written to, and returned? 
        //              Also need to keep track of line count to provide 
        //              context for those errors. 
        switch(ReadAsmString->Contents[ReadIndex])
        {
            // NOTE(Marko): Ignore everything until end of line if there is 
            //              indeed a comment
            case COMMENT_SLASH:
            {
                if((ReadIndex < ReadAsmString->Length - 1) &&
                   (ReadAsmString->Contents[ReadIndex + 1] == COMMENT_SLASH))
                {
                    while(ReadAsmString->Contents[ReadIndex] != NEWLINE)
                    {
                        ReadIndex++;
                        RemovedCharsCount++;
                    }
                }
                else
                {
                    // TODO(Marko): Error handling -- we have a floating slash
                }
            } break;

            case A_INSTRUCTION_SYMBOL:
            {
                // NOTE(Marko): If the character immediately following the 
                //              A_INSTRUCTION_SYMBOL is a number, then we have 
                //              a direct memory access, and can simply copy it 
                //              over. Thus we check for the case where it is 
                //              NOT a number, in which case we need to check 
                //              if it is in the predefined variable table. If 
                //              it's not, then we increment the count of
                //              user-defined variables, which we will use to
                //              allocate memory for a user-defined variable 
                //              table later. 
                if((ReadIndex < ReadAsmString->Length - 1) && 
                   (!IsCharNumber(ReadAsmString->Contents[ReadIndex+1])))
                {
                    asm_string AInstructionSymbol;
                    // NOTE(Marko): AInstructionSymbol.Contents now points to 
                    //              already-allocated memory. 
                    AInstructionSymbol.Contents = &ReadAsmString->Contents[ReadIndex+1];
                    AInstructionSymbol.Length = 0;
                    char *CurrentChar = AInstructionSymbol.Contents;
                    // NOTE(Marko): Go to the end of the word
                    while((*CurrentChar != NEWLINE) && 
                          (*CurrentChar != WHITESPACE) &&
                          (*CurrentChar != TAB) &&
                          (*CurrentChar != CARRIAGE_RETURN))
                    {
                        AInstructionSymbol.Length++;
                        CurrentChar++;
                    }
                    // TODO(Marko): Replace the predefined variables with 
                    //              their addresses here instead of in the 
                    //              second pass. 
                    if(!IsInVariableTable(PredefinedVariableTable, 
                                          &AInstructionSymbol))
                    {
                        UserDefinedVariableCount++;
                    }
                }
            } break;

            // NOTE(Marko): Do not copy over whitespaces, Tabs, and Carriage 
            //              Returns. While I could skip writing the break; and 
            //              just do a fall through, that would make the code a 
            //              bit harder to read since when I read the code, I 
            //              usually expect to see a break; at the end of each 
            //              case statement. 
            case WHITESPACE:
            {
                ReadIndex++;
                RemovedCharsCount++;
            } break;

            case TAB:
            {
                ReadIndex++;
                RemovedCharsCount++;
            } break;

            case CARRIAGE_RETURN:
            {
                ReadIndex++;
                RemovedCharsCount++;
            } break;

            // NOTE(Marko): Remove duplicate newlines
            case NEWLINE:
            {
                if((ReadIndex < ReadAsmString->Length - 1) &&
                   (ReadAsmString->Contents[ReadIndex+1] == NEWLINE))
                {
                    ReadIndex++;
                    RemovedCharsCount++;
                }
            } break;


            // TODO(Marko): We can check for illegal whitespace inside a 
            //              bracket for errors. 
            // TODO(Marko): Make sure the previous char is a newline (i.e. 
            //              you're not dealing with a random bracket in the 
            //              middle of a line) 
            case OPEN_BRACKET:
            {   
                // NOTE(Marko): Found a label. 
                LabelCount++;
            } break;

            default:
            {
                // NOTE(Marko): Nothing happens here by default -- we simply 
                //              exit the switch statement and proceed to copy 
                //              the character.
            }
        }
        // NOTE(Marko): After handling the current char and moving ReadIndex 
        //              where necessary, copy over the character from the old 
        //              string to the new string (take note that they are 
        //              using different indices so we need to manually 
        //              increment WriteIndex)
        WriteAsmString->Contents[WriteIndex] = 
            ReadAsmString->Contents[ReadIndex];
        WriteIndex++;
    }

    // NOTE(Marko): Adjust the length of both strings
    WriteAsmString->Length -= RemovedCharsCount;
    ReadAsmString->Length -= RemovedCharsCount;

    DebugPrintAsmString(WriteAsmString); 

    OutputDebugString("User-defined Variable Count: ");
    DebugPrintUInt32(UserDefinedVariableCount);


    InitializeUserDefinedVariableTable(UserDefinedVariableTable, 
                                       UserDefinedVariableCount);

    // NOTE(Marko): Creating a label table. This LabelTabel will be freed at
    //              the end of this function

    variable_table LabelTable = CreateLabelTable(LabelCount);


    //
    // NOTE(Marko): Second pass
    //

    // NOTE(Marko): Swap the AsmStrings so that we can continue selectively 
    //              copying from Old to New
    asm_string *Temp = ReadAsmString;
    ReadAsmString = WriteAsmString;
    WriteAsmString = Temp;

    
    TotalRemovedCharsCount += RemovedCharsCount;
    RemovedCharsCount = 0;
    WriteIndex = 0;
    *LineCount = 0;
    uint32 CurrentUserDefinedIndex = 0;
    uint32 CurrentLabelIndex = 0;
    
    // NOTE(Marko): This is where we basically remove all the leading newline 
    //              chars -- we start the for loop through ReadAsmString from 
    //              the first non-newline char
    uint32 FirstNonNewLineCharIndex = 0;
    while(ReadAsmString->Contents[FirstNonNewLineCharIndex] == NEWLINE)
    {
        RemovedCharsCount++;
        FirstNonNewLineCharIndex++;
    }

    for(uint32 ReadIndex = FirstNonNewLineCharIndex; 
        ReadIndex < ReadAsmString->Length; 
        ReadIndex++)
    {
        
        switch(ReadAsmString->Contents[ReadIndex])
        {
            case CARRIAGE_RETURN:
            {
                // NOTE(Marko): We should've gotten rid of these in the first 
                //              pass
                InvalidCodePath;
            } break;

            case WHITESPACE:
            {
                // NOTE(Marko): We should've gotten rid of these in the first 
                //              pass
                InvalidCodePath;
            } break;

            case TAB:
            {
                // NOTE(Marko): We should've gotten rid of these in the first 
                //              pass
                InvalidCodePath;
            } break;

            case NEWLINE:
            {
                // NOTE(Marko): Remove consecutive newlines
                while((ReadIndex < ReadAsmString->Length-1) && 
                      (ReadAsmString->Contents[ReadIndex+1] == NEWLINE))
                {
                    ReadIndex++;
                    RemovedCharsCount++;
                }
                WriteAsmString->Contents[WriteIndex] = 
                    ReadAsmString->Contents[ReadIndex];
                WriteIndex++;
                (*LineCount)++;  
            } break;

            case A_INSTRUCTION_SYMBOL:
            {
                // NOTE(Marko): First copy the @ over
                WriteAsmString->Contents[WriteIndex] = 
                    ReadAsmString->Contents[ReadIndex];
                WriteIndex++;
                // NOTE(Marko): Then check if the next symbol is a number
                if((ReadIndex <ReadAsmString->Length - 1) &&
                   (!IsCharNumber(ReadAsmString->Contents[ReadIndex+1]))) 
                {
                    asm_string AInstructionSymbol;
                    // NOTE(Marko): AInstructionSymbol.Contents now points to 
                    //              already-allocated memory. No need to alloc 
                    //              or free
                    AInstructionSymbol.Contents = &ReadAsmString->Contents[ReadIndex+1];
                    AInstructionSymbol.Length = 0;
                    char *CurrentChar = AInstructionSymbol.Contents;
                    // NOTE(Marko): Go to the end of the word
                    while(*CurrentChar != NEWLINE)
                    {
                        AInstructionSymbol.Length++;
                        CurrentChar++;
                    }
                    uint32 FoundIndex = 0;
                    if(WhereInVariableTable(PredefinedVariableTable, 
                                            &AInstructionSymbol,
                                            &FoundIndex))
                    {
                        // NOTE(Marko): Retrieve the matched entry, stringify 
                        //              the VariableAddress, and copy that 
                        //              into WriteAsmString, then increment 
                        //              ReadAsmString to just before NEWLINE. 
                        uint16 VariableAddress = PredefinedVariableTable->VariableAddresses[FoundIndex];
                        asm_string StringedVariableAddress = 
                            UInt16ToAsmString(VariableAddress);

                        CopyString(StringedVariableAddress.Contents,
                                   StringedVariableAddress.Length,
                                   WriteAsmString->Contents + WriteIndex,
                                   WriteAsmString->Length - RemovedCharsCount);

                        
                        // NOTE(Marko): We've removed the AInstruction Symbol 
                        //              here and replaced it with a numerical 
                        //              address.
                        RemovedCharsCount += (AInstructionSymbol.Length - StringedVariableAddress.Length);
                        WriteIndex += StringedVariableAddress.Length;

                        while(ReadAsmString->Contents[ReadIndex] != NEWLINE)
                        {
                            ReadIndex++;
                        }
                        // NOTE(Marko): We've seeked to a newline 
                        //              decrement ReadIndex. Then the switch 
                        //              statement can handle the newline
                        ReadIndex--;

                    }
                    else if(WhereInVariableTable(UserDefinedVariableTable,
                                                 &AInstructionSymbol,
                                                 &FoundIndex))
                    {
                        // NOTE(Marko): We have found a match in 
                        //              UserDefinedVariableTable. At this 
                        //              point, all the addresses here 
                        //              should be UNINITIALIZED_ADDRESS 
                        //              (because we haven't yet used the 
                        //              LabelTable to figure out the 
                        //              addresses). So we do nothing. 

                    }
                    else
                    {
                        // NOTE(Marko): This is our first time seeing this 
                        //              VariableName, so add it to the 
                        //              UserDefinedVariableTable and increment 
                        //              the CurrentUserDefinedIndex.
                        AddVariableToVariableTable(UserDefinedVariableTable,
                                                   CurrentUserDefinedIndex,
                                                   &AInstructionSymbol,
                                                   UNINITIALIZED_ADDRESS);

                        CurrentUserDefinedIndex++;
                    }

                }
            } break;

            case OPEN_BRACKET:
            {
                asm_string LabelSymbol;
                // NOTE(Marko): LabelSymbol.Contents now points to 
                //              already-allocated memory. No need to alloc or 
                //              free. 
                LabelSymbol.Contents = &ReadAsmString->Contents[ReadIndex+1];
                LabelSymbol.Length = 0;
                char *CurrentChar = LabelSymbol.Contents;
                while(*CurrentChar != CLOSED_BRACKET)
                {
                    LabelSymbol.Length++;
                    CurrentChar++;
                }

                // NOTE(Marko): Add the LabelSymbol to the LabelTable 
                //              using the LineCount. Then decrement LineCount 
                //              because the label is not supposed to be a 
                //              program instruction (so it doesn't count as a 
                //              line)! 
                AddVariableToVariableTable(&LabelTable,
                                           CurrentLabelIndex,
                                           &LabelSymbol,
                                           *LineCount);
                CurrentLabelIndex++;
                (*LineCount)--;
                WriteAsmString->Contents[WriteIndex] = 
                    ReadAsmString->Contents[ReadIndex];
                WriteIndex++; 
            } break;

            default:
            {
                // NOTE(Marko): Copy over the character from the old string to 
                //              the new string (take note that they are using 
                //              different indices so we need to manually 
                //              increment WriteIndex)
                WriteAsmString->Contents[WriteIndex] = 
                    ReadAsmString->Contents[ReadIndex];
                WriteIndex++;               
            }
        }
    }

    TotalRemovedCharsCount += RemovedCharsCount;
    // NOTE(Marko): Again we reduce the length of both strings by the number 
    //              of characters that were removed. 
    ReadAsmString->Length -= RemovedCharsCount;
    WriteAsmString->Length -= RemovedCharsCount;
    // NOTE(Marko): Swap the pointers back again now that the second pass is 
    //              done. ReadAsmString now points back to the memory that it 
    //              first pointed to when it was passed into this function. 
    Temp = ReadAsmString;
    ReadAsmString = WriteAsmString;
    WriteAsmString = Temp;

    // NOTE(Marko): Iterate through the Label Table to fill in the addresses 
    //              of the Variable Table. 
    for(uint32 LabelIndex = 0; LabelIndex < LabelTable.Size; LabelIndex++)
    {
        uint32 FoundIndex = 0;
        if(WhereInVariableTable(UserDefinedVariableTable, 
                        &LabelTable.VariableNames[LabelIndex], 
                        &FoundIndex))
        {
            UserDefinedVariableTable->VariableAddresses[FoundIndex] = 
                LabelTable.VariableAddresses[LabelIndex];
        }
    }

    // NOTE(Marko): Iterate through the Variable Table to fill in all 
    //              remaining uninitialized variable addresses. 
    uint16 CurrentUserDefinedAddress = USER_DEFINED_VAR_ADDRESS_START;
    for(uint32 UserDefinedIndex = 0; 
        UserDefinedIndex < UserDefinedVariableTable->Size; 
        UserDefinedIndex++)
    {
        if(UserDefinedVariableTable->VariableAddresses[UserDefinedIndex] == 
            UNINITIALIZED_ADDRESS)
        {
            UserDefinedVariableTable->VariableAddresses[UserDefinedIndex] = 
                CurrentUserDefinedAddress;
            CurrentUserDefinedAddress++;
        }
    }

    // NOTE(Marko): We no longer need Label Table, so we free it. 
    FreeVariableTable(&LabelTable);

    // NOTE(Marko): IMPORTANT: Make sure you free the correct memory! This 
    //                         depends on how many times you swapped pointers. 
    //                         For odd numbers of passes, free ReadAsmString. 
    //                         For even numbers of passes, free 
    //                         WriteAsmString. As of this comment 
    //                         (2021-12-13), we have two passes, so we free 
    //                         WriteAsmString.
    FreeAsmString(WriteAsmString);
}
