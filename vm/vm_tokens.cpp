#include "vm_tokens.h"
#include "vm_string.h"


vm_string ParseReturnCommand(void)
{
    vm_string Result = {0};

    return(Result);
}


vm_string ParseArithmeticCommand(vm_string *VMStringArithmeticCommand)
{
    vm_string Result = {0};
    if(VMStringsAreEqual(VMStringArithmeticCommand, "add", 3))
    {
        /* NOTE(Marko): add translates to:
                            @SP
                            M=M-1
                            A=M
                            D=M
                            @SP
                            M=M-1
                            A=M
                            M=M+D
                            @SP
                            M=M+1
        */
        Result.Contents = "@SP\nM=M-1\nA=M\nD=M\n@SP\nM=M-1\nA=M\nM=M+D\n@SP\nM=M+1\n";
        Result.CurrentLength = 48;
        Result.MemorySize = 49;
    }

    return(Result);
}

vm_string ParseTokensToAsm(vm_tokens *VMTokens)
{
    vm_string Result = {0};
    // TODO(Marko): Figure out if we should first examine 
    //              VMTokens->VMTokenCount before going through the tokens as 
    //              a preliminary filter (e.g. if VMTokens->VMTokenCount == 1, 
    //              then check only for "add" "sub", "eq" etc)
    // TODO(Marko): Parse "add" "push constant %d" -- this will take care of 
    //              SimpleAdd.vm

    switch(VMTokens->VMTokenCount)
    {
        case 1:
        {
            if(!VMStringsAreEqual(&VMTokens->VMTokens[0], "return", 6))
            {
                Result = ParseArithmeticCommand(&VMTokens->VMTokens[0]);
            }
            else
            {
                Result = ParseReturnCommand();
            }
        } break;
    }


    return(Result);
}

vm_tokens TokenizeLine(vm_string *VMInputString, 
                       uint32 *InputIndex)
{
    // TODO(Marko): Consider scoping the two passes separately.
    // TODO(Marko): Is there a way to do this in one pass? 
    //
    // NOTE(Marko): Counting how many tokens on the line. 
    //
    vm_tokens Result = {0};
    uint32 TokenCount = 0;
    char *CurrentChar = &VMInputString->Contents[*InputIndex];
    while(*CurrentChar != NEWLINE)
    {
        if(*CurrentChar == WHITESPACE)
        {
            TokenCount++;
        }
        CurrentChar++;
    }
    // NOTE(Marko): Final token is deliniated using \n so add one more to 
    //              TokenCount
    TokenCount++;
    //
    // NOTE(Marko): Allocate the vm_string structs
    //
    Result.VMTokenCount = TokenCount;
    Result.VMTokens = 
        (vm_string *)malloc(Result.VMTokenCount*sizeof(vm_string));

    // NOTE(Marko): Putting tokens into vm_tokens struct. 
    CurrentChar = &VMInputString->Contents[*InputIndex];
    uint32 TokenIndex = 0;
    while(*CurrentChar != NEWLINE)
    {
        char *CurrentTokenBegin = CurrentChar;
        while((*CurrentChar != WHITESPACE) && (*CurrentChar != NEWLINE))
        {
            (*InputIndex)++;
            CurrentChar++;
        }
        char *CurrentTokenEnd = CurrentChar; 

        uint32 CurrentTokenLength = 
            (uint32)(CurrentTokenEnd - CurrentTokenBegin);

        vm_string CurrentToken = AllocateVMString(CurrentTokenLength+1);
        CurrentToken.CurrentLength = CurrentTokenLength;
        CopyVMString(CurrentTokenBegin, 
                     CurrentTokenLength,
                     CurrentToken.Contents,
                     CurrentTokenLength);
        // NOTE(Marko): Null terminate
        CurrentToken.Contents[CurrentToken.CurrentLength] = '\0';

        Result.VMTokens[TokenIndex] = CurrentToken;
        TokenIndex++;

        if(*CurrentChar != NEWLINE)
        {
            CurrentChar++;
            (*InputIndex)++;
        }
    }

    return(Result);
}