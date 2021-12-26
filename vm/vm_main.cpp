#include "vm_main.h"

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: supply the *.vm file to be translated as the first\n");
        printf("argument, and supply the target output file as the second\n");
        printf("argument.\n");

        return(1);
    }
    char *InputFileName = argv[1];
    char *OutputFileName = argv[2];

    read_file_result InputFileContents = ReadEntireFile(InputFileName);
    if(InputFileContents.Length > 0)
    {

    }

    FreeEntireFileMemory(InputFileContents); 

    return(0);
}