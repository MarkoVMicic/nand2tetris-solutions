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




internal void PreprocessAsmString(asm_string *AsmString)
{

    RemoveComments(AsmString);

}
