#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocess.h"
// #include "parse.h"

int main(int argc, char **argv)
{
	char *file_path;
	char *asm_string;


	if(argc != 2)
	{
		printf("Usage: Supply the file path of the assembly file to be ");
		printf("translated into machine code as the first argument of ");
		printf("this program.\n ");
		return(1);
	}
	// program_name = argv[0];
	file_path = argv[1];
	if(file_path[0] == '\0')
	{
		printf("File_path is an empty string or the first character is \\0.");
		printf("\n");
		printf("Aborting program... ");
		return(1);
	}
	asm_string = open_file_store_as_string(file_path);
	// Abort the program if no file was found. 
	if(asm_string[0] == '\0')
	{
		printf("Failed to load file at %s. Aborting program...\n", file_path);
		return 1;
	}
	asm_string = process_asm_string(asm_string);
	// puts(asm_string);
	// parse_asm_string(asm_string);

	free(asm_string);			// Freeing the malloc'd string

	return(0);
};