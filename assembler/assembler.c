#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocess.h"
#include "parse.h"

int main(int argc, char **argv)
{
	char *input_file_path;
	char *output_file_path;
	char *asm_string;


	if(argc != 3)
	{
		printf("Usage: Supply the file path of the assembly file to be ");
		printf("translated into machine code as the first argument of ");
		printf("this program.\n ");
		printf("Supply the file path of the machine-code output file as the");
		printf("second argument of this program.\n ");
		return(1);
	}
	// program_name = argv[0];
	input_file_path = argv[1];
	output_file_path = argv[2];
	if(input_file_path[0] == '\0')
	{
		printf("File_path is an empty string or the first character is \\0.");
		printf("\n");
		printf("Aborting program... ");
		return(1);
	}
	asm_string = open_file_store_as_string(input_file_path);
	// Abort the program if no file was found. 
	if(asm_string[0] == '\0')
	{
		printf("Failed to load file at %s. Aborting program...\n", input_file_path);
		return 1;
	}
	// puts("Before processing.\n");
	// puts(asm_string);
	asm_string = process_asm_string(asm_string);
	puts("Finished preprocessing");
	// puts("After processing.\n");
	// puts(asm_string);
	// TODO(Marko): Consider making this the second arg of the main program instead of just 
	// 				#define it here. 
	parse_asm_string(asm_string, output_file_path);
	printf("Finished parsing and writing to file at %s\n", output_file_path);


	// Freeing the malloc'd string	
	free(asm_string);

	return(0);
};