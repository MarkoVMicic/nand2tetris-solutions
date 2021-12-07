#include "preprocess.h"

internal void RemoveComments(asm_string *AsmString)
{
    // NOTE(Marko): ASM comments are identical to single-line comments in C.

    for(int i = 0; i < AsmString->Length; i++)
    {
        // NOTE(Marko): Look for two consecutive slashes
        // TODO(Marko): Can AsmString->Contents[i+1] access unallocated 
        //              memory? Investigate
        if(AsmString->Contents[i] == '/' && AsmString->Contents[i+1] == '/')
        {
            // NOTE(Marko): Double slash found, so everything until the end of 
            //              the line is a comment. 
            int j = i;
            while(AsmString->Contents[j] != '\n')
            {
                j++;
            }
            for(int k = 0; k < j-i; k++)
            {
                // NOTE(Marko): Since we later remove all whitespace, its 
                //              convenient to just replace all the comments 
                //              with whitespace. 
                AsmString->Contents[i+k] = ' ';
            }
        }
    }
}


// TODO(Marko): Rename Count to something more sensible.
internal void RemoveWhiteSpace(asm_string *AsmString)
{
    int Count = 0;
    int RemovedCharCount = 0;
    for(int i = 0; i < AsmString->Length; i++)
    {
        // NOTE(Marko): If we encounter a space, we will remove it. This shortens the string length. If we don't encounter a space, we'll copy whatever is at the ith position into the count position. 
        if(AsmString->Contents[i] != ' ')
        {
            AsmString->Contents[Count++] = AsmString->Contents[i];
        }
        else
        {
            RemovedCharCount++;
        }
    }
    // NOTE(Marko): Need to null terminate the string.
    AsmString->Contents[Count] = '\0';
    AsmString->Length -= RemovedCharCount;
}


internal void RemoveTabs(asm_string *AsmString)
{
    int Count = 0;
    int RemovedCharCount = 0;
    for(int i = 0; i < AsmString->Length; i++)
    {
        if(AsmString->Contents[i] == '\t')
        {
            RemovedCharCount++;
        }
        else
        {
            AsmString->Contents[Count++] = AsmString->Contents[i];
        }
    }
    AsmString->Contents[Count] = '\0';
    AsmString->Length -= RemovedCharCount;
}


internal void RemoveBlankLines(asm_string *AsmString)
{
    int Count = 0;
    int RemovedCharCount = 0;
    for(int i = 0; i < AsmString->Length; i++)
    {
        // NOTE(Marko): Remove Carriage Returns
        if(AsmString->Contents[i] == '\r')
        {
            RemovedCharCount++;
        }
        // NOTE(Marko): Remove consecutive newlines
        else if((i > 0) && (AsmString->Contents[i] == '\n') && AsmString->Contents[i-1] == '\n')
        {
            RemovedCharCount++;
        }
        else
        {
            AsmString->Contents[Count++] = AsmString->Contents[i];
        }
    }
    // NOTE(Marko): Need to null terminate the string.
    AsmString->Contents[Count] = '\0';
    AsmString->Length -= RemovedCharCount;
}

internal bool32 IsCharNumber(char Char)
{
    bool32 Result = ((Char == '0') &&
                     (Char == '1') &&
                     (Char == '2') &&
                     (Char == '3') &&
                     (Char == '4') &&
                     (Char == '5') &&
                     (Char == '6') &&
                     (Char == '7') &&
                     (Char == '8') &&
                     (Char == '9'));

    return(Result);
} 


internal bool32 IsInVariableTable(variable_table *VariableTable, 
                                  asm_string *AsmString)
{                                  
    bool32 Result = false; 
}



internal void PreprocessAsmString(asm_string *OldAsmString)
{
    asm_string NewAsmString;
    NewAsmString.Length = OldAsmString->Length;
    NewAsmString.Contents = 
        (char *)VirtualAlloc(0, 
                             OldAsmString->Length*sizeof(char),
                             MEM_RESERVE | MEM_COMMIT, 
                             PAGE_READWRITE);

    variable_table PredefinedVariableTable = CreatePredefinedVariableTable();

    uint32 NewIndex = 0;
    uint32 RemovedChars = 0;
    uint32 UserDefinedVariableCount = 0;
    for(uint32 OldIndex = 0; OldIndex < OldAsmString->Length; OldIndex++)
    {
        switch(OldAsmString->Contents[OldIndex])
        {
            case COMMENT_SLASH:
            {
                if((OldIndex < OldAsmString->Length - 1) &&
                   (OldAsmString->Contents[OldIndex + 1] == COMMENT_SLASH))
                {
                    while(OldAsmString->Contents[OldIndex] != NEWLINE)
                    {
                        OldIndex++;
                        RemovedChars++;
                    }
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
                   (!IsCharNumber(OldAsmString->Contents[OldIndex])))
                {
                    uint32 AInstructionSymbolLength = 0;
                    asm_string AInstructionSymbol;
                    // NOTE(Marko): AInstructionSymbol.Contents now points to 
                    //              already-allocated memory. 
                    AInstructionSymbol.Contents = &OldAsmString->Contents[OldIndex+1];
                    char *CurrentChar = AInstructionSymbol.Contents;
                    while(*CurrentChar != NEWLINE)
                    {
                        AInstructionSymbol.Length++;
                        CurrentChar++;
                    }
                    if(!IsInVariableTable(&PredefinedVariableTable, 
                                          &AInstructionSymbol))
                    {
                        UserDefinedVariableCount++;
                    }
                }
            } break;

            default:
            {
                // NOTE(Marko): Copy over the character from the old string to 
                //              the new string (take note that they are using 
                //              different indices so we need to manually 
                //              increment NewIndex)
                NewAsmString.Contents[NewIndex] = 
                    OldAsmString->Contents[OldIndex];
                NewIndex++;
            }
        }
    }
    NewAsmString.Length -= RemovedChars;

    /*
        Selectively copy characters from OldAsmString to NewAsmString. In 
        particular: 

        1. // = comment -> remove entirely until newline

        2. @  = A-instruction. Check if next is number or not.
                - If number, go to end of line (i.e. ignore it)
                - If !number, go to end of word, and check if there is a match in predefined variable table. 
                    - If there is a match, go to end of line
                    - If there isn't a match, increment the UserDefinedVariablesCounter

        3. Remove all whitespace (how?)

        4. Everything else should be a D-instruction, just go to the next 
           line. 
    */

    RemoveComments(OldAsmString);
    RemoveWhiteSpace(OldAsmString);
    RemoveTabs(OldAsmString);
    RemoveBlankLines(OldAsmString);
}
