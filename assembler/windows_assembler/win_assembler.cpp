#include <stdio.h>
#include <windows.h>
#include "win_assembler.h"


// TODO(Marko): Try to figure out how to use WinMain() properly here 
//              (for learning, not practicality)
int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: Supply the file path of the assembly file to be ");
        printf("translated into machine code as the first argument of ");
        printf("this program.\n ");
        printf("Supply the file path of the machine-code output file as the");
        printf("second argument of this program.\n ");
        printf("\n\n");
        printf("Aborting program...\n");
        return(1);
    }

    // char *ProgramName = argv[0];
    char *InputFilePath = argv[1];
    char *OutputFilePath = argv[2];



    return(0);
}