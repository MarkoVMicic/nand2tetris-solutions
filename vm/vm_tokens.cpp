#include "vm_tokens.h"
#include "vm_string.h"

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