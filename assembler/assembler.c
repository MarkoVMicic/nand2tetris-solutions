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
	/* 
		TODO(Marko): Do these two strings for the file-paths that are passed 
					as args in the command-line when the program is executed 
					need to be allocated and/or null-terminated, or does C 
					actually handle all of that for us? The program works just 
					fine without needing to allocate or null-terminate any of 
					the strings, so presumably C handles it. 
	*/
	// program_name = argv[0];
	input_file_path = argv[1];
	output_file_path = argv[2];
	if(input_file_path[0] == '\0')
	{
		printf("Input File path is an empty string or the first character is 
				'\\0'.");
		printf("\n");
		printf("Aborting program... ");
		return(1);
	}
	if(output_file_path[0] == '\0')
	asm_string = open_file_store_as_string(input_file_path);
	// Abort the program if no file was found. 
	if(asm_string[0] == '\0')
	{
		printf("Failed to load file at %s. Aborting program...\n", input_file_path);
		return 1;
	}
	asm_string = process_asm_string(asm_string);
	parse_asm_string(asm_string, output_file_path);
	printf("Finished parsing and writing to file at %s\n", output_file_path);


	// Freeing the malloc'd string	
	free(asm_string);

	return(0);
};