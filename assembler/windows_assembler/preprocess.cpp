#include "preprocess.h"

// TODO(Marko): We can probably make a SwapPointers() macro. Figure it out if 
//              you have some time.
// TODO(Marko): Conversely, perhaps we should scrap this function and just do 
//              it inline, because this function forces us to copy all of the 
//              data in the structs around instead of just swapping the 
//              pointers... 
inline void SwapAsmStringPointers(asm_string *A, asm_string *B)
{
    asm_string Temp = *A;
    *A = *B;
    *B = Temp;
}


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
internal void PreprocessAsmString(asm_string *OldAsmString, 
                                  asm_string *NewAsmString,
                                  variable_table *PredefinedVariableTable,
                                  variable_table *UserDefinedVariableTable)
{

    //
    // NOTE(Marko): First pass    
    //

    uint32 NewIndex = 0;
    uint32 RemovedCharsCount = 0;
    uint32 UserDefinedVariableCount = 0;
    for(uint32 OldIndex = 0; OldIndex < OldAsmString->Length; OldIndex++)
    {
        // TODO(Marko): pull out the copying of the character into an inline 
        //              function? This would avoid code copy-pasta
        // TODO(Marko): This is a useful place to start recording errors. What 
        //              would we need to do that? An error struct maybe, which 
        //              gets passed into here, and written to, and returned? 
        //              Also need to keep track of line count to provide 
        //              context for those errors. 
        switch(OldAsmString->Contents[OldIndex])
        {
            // NOTE(Marko): Ignore everything until end of line if there is 
            //              indeed a comment
            // TODO(Marko): Multi-line comments leave a trail of \n. It's not 
            //              important to deal with this, but it would be nice 
            //              to. 
            case COMMENT_SLASH:
            {
                if((OldIndex < OldAsmString->Length - 1) &&
                   (OldAsmString->Contents[OldIndex + 1] == COMMENT_SLASH))
                {
                    while(OldAsmString->Contents[OldIndex] != NEWLINE)
                    {
                        OldIndex++;
                        RemovedCharsCount++;
                    }
                    // NOTE(Marko): Copy the newline char into the 
                    //              NewAsmString (take note that they are 
                    //              using different indices so we need to 
                    //              manually increment NewIndex)
                    NewAsmString->Contents[NewIndex] = 
                        OldAsmString->Contents[OldIndex];
                    NewIndex++;
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
                if((OldIndex < OldAsmString->Length - 1) && 
                   (!IsCharNumber(OldAsmString->Contents[OldIndex+1])))
                {
                    asm_string AInstructionSymbol;
                    // NOTE(Marko): AInstructionSymbol.Contents now points to 
                    //              already-allocated memory. 
                    AInstructionSymbol.Contents = &OldAsmString->Contents[OldIndex+1];
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
                    if(!IsInVariableTable(PredefinedVariableTable, 
                                          &AInstructionSymbol))
                    {
                        UserDefinedVariableCount++;
                    }
                }
                // NOTE(Marko): Copy over the character from the old string to 
                //              the new string (take note that they are using 
                //              different indices so we need to manually 
                //              increment NewIndex)
                NewAsmString->Contents[NewIndex] = 
                    OldAsmString->Contents[OldIndex];
                NewIndex++;
            } break;

            // NOTE(Marko): Do not copy over whitespaces, Tabs, and Carriage 
            //              Returns. While I could skip writing the break; and 
            //              just do a fall through, that would make the code a 
            //              bit harder to read since when I read the code, I 
            //              usually expect to see a break; at the end of each 
            //              case statement. 
            case WHITESPACE:
            {
                RemovedCharsCount++;
            } break;

            case TAB:
            {
                RemovedCharsCount++;
            } break;

            case CARRIAGE_RETURN:
            {
                RemovedCharsCount++;
            } break;

            // NOTE(Marko): Remove duplicate newlines
            case NEWLINE:
            {
                if((OldIndex < OldAsmString->Length - 1) &&
                   (OldAsmString->Contents[OldIndex+1] == NEWLINE))
                {
                    RemovedCharsCount++;
                }
                else
                {
                    // NOTE(Marko): Copy over the character from the old 
                    //              string to the new string (take note that 
                    //              they are using 
                    //              different indices so we need to manually 
                    //              increment NewIndex)
                    NewAsmString->Contents[NewIndex] = 
                        OldAsmString->Contents[OldIndex];
                    NewIndex++; 
                }
            } break;

            default:
            {
                // NOTE(Marko): Copy over the character from the old string to 
                //              the new string (take note that they are using 
                //              different indices so we need to manually 
                //              increment NewIndex)
                NewAsmString->Contents[NewIndex] = 
                    OldAsmString->Contents[OldIndex];
                NewIndex++;
            }
        }
    }
    NewAsmString->Length -= RemovedCharsCount;

    DebugPrintAsmString(NewAsmString); 

    OutputDebugString("User-defined Variable Count: ");
    DebugPrintUInt32(UserDefinedVariableCount);

    InitializeUserDefinedVariableTable(UserDefinedVariableTable, 
                                       UserDefinedVariableCount);

    //
    // NOTE(Marko): Second pass
    //

    // NOTE(Marko): Swap the AsmStrings so that we can continue selectively 
    //              copying from Old to New
    SwapAsmStringPointers(OldAsmString, NewAsmString);

}
