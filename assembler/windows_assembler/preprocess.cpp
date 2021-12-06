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


internal void PreprocessAsmString(asm_string *AsmString)
{
    // TODO(Marko): Should be able to do all of this in a single pass instead of multiple passes like this. 
    RemoveComments(AsmString);
    RemoveWhiteSpace(AsmString);
    RemoveTabs(AsmString);
    RemoveBlankLines(AsmString);
}
