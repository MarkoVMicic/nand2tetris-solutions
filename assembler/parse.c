#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILE_PATH "/Users/markomicic/Desktop/NAND_to_tetris_coursera/nand2tetris/projects/06/assembler/test.hack"

/*	
C-instruction format:
	DEST = COMP ; JUMP

	DEST:
		Can only take one of these values: 
			'A', 'M', 'D', 'AM', 'MA', 'AD', 'DA', 'MD', 'DM', 'AMD', 'ADM', 'MAD', 'MDA', 'DAM', 'DMA'
	COMP:
		Can begin with any of these characters:
			'0', '1', '-1', 'D', 'A', 'M', '!', '-'

*/

void parse_c_instruction(char * current_line){
	char machine_code_instruction_string[17];
	char comp_string[7];
	char dest_string[3];
	char jump_string[3];
	char *equals_sign_postion; 
	int i;

	// For C-instruction, the 3 most significant bits are always set to 1. 
	// Again, endianness of C is opposite to Hack computer.
	machine_code_instruction_string[0] = '1';
	machine_code_instruction_string[1] = '1';
	machine_code_instruction_string[2] = '1';

	// Find first occurence of equal sign using strchr(). If strchr() returns NULL, 
	// then there is no dest in the C-instruction, so we can fill the comp_string with 
	// 0s.
	equals_sign_postion = strchr(current_line, '=');
	if(equals_sign_postion == NULL)
	{
		for(i=0; i<3; i++){
			dest_string[i] = '0';
		}
	}


	
	for(i=0; i<3; i++){
		machine_code_instruction_string[12-i] = dest_string[i];
	}
	machine_code_instruction_string[16] = '\0'; // Null terminate the string. 
}

// Not strictly necessary since we can simply output the machine code in reverse using a for loop. 
// Mainly implementing this for coding practice. 
int * reverse_array_order(int * array){
	int array_length = sizeof(array) / sizeof(array[0]);
	int temp[array_length];
	int i;

	for(i=0; i < array_length; i++){
		temp[i] = array[array_length-i];
	}
	for(i=0; i < array_length; i++){
		array[i] = temp[i];
	}
	return array;
}

void parse_a_instruction(char * current_line){
	// TODO(Marko): This is naive implementation which only supports A-instructions that enumerate the address using a number. 
	//				Likely we will have to expand this using yet another switch statement and a table.
	// Note: C arrays output from left to right, whereas machine code instructions read from right to left 
	// (different endianness). Thus we need to reverse the instruction we get. 
	int machine_code_instruction[16] = {0};
	int length_of_current_line = strlen(current_line);
	int i;
	char * A_instruction_address;
	int A_instruction_decimal;
	char machine_code_instruction_string[18];
	


	A_instruction_address = current_line + 1;
	A_instruction_decimal = atoi(A_instruction_address);
	// This creates a "backwards" array of binary ints. 
	for(i=0; A_instruction_decimal > 0; i++){

		machine_code_instruction[i] = A_instruction_decimal % 2;
		A_instruction_decimal = A_instruction_decimal / 2;
		if(i == 15){
			// Prevent going out of bounds
			break;
		}
	}
	machine_code_instruction[15] = 0;	// Op-code = 0.
	// Reverse the array. Using inline function here, mainly coz I have no idea how to make it into a function! 
	// TODO(Marko): Figure out how to turn this into a function which takes in an array and returns the reverse of that array. 
	{
		int temp;
		int start = 0;
		int end = 15; 
	    while (start < end) 
	    { 
	        temp = machine_code_instruction[start];    
	        machine_code_instruction[start] = machine_code_instruction[end]; 
	        machine_code_instruction[end] = temp; 
	        start++; 
	        end--; 
	    }    
	}
	for(i=0; i<16; i++){
		// The below line is a little bit of an evil hack. Basically, machine_code_instruction[i] is an int, and 
		// so by abusing ASCII, we can add '0' to the int to convert it into its string equivalent. 
		machine_code_instruction_string[i] = machine_code_instruction[i] + '0';
	}
	machine_code_instruction_string[16] = '\n';	// Add newline to the end of the string
	machine_code_instruction_string[17] = '\0'; // Null terminate the string. 
	// Write to output file. 
	{
		FILE * output_file;

		output_file = fopen(OUTPUT_FILE_PATH, "a");
		fputs(machine_code_instruction_string, output_file);
	}



}

void parse_current_line(char * current_line){
	char first_character;
	first_character = current_line[0];
	switch(first_character)
	{
		case '@':
			{
			parse_a_instruction(current_line);
			};	break;
		// TODO(Marko): Implement error handling for bad strings, and expand this switch
		//				statement so that it encompasses all the illegal beginnings of a string. 
		default :
			{
			parse_c_instruction(current_line);
			}
	}
}

const char * parse_line_and_update_to_next_line(const char *current_line){
	const char * next_line;
	char * temp_string;
	int current_line_length;
	char stop_string[100];

	// Look for the next-line position. 
	next_line = strchr(current_line, '\n');
	// If next_line is not null, then the string length is next_line - current_line (pointer arithmetic),
	// otherwise, the current line length is simply the length of the current_line, because we couldn't
	// find a carriage return (\n) anywhere in the current line.   
	current_line_length = next_line ? (next_line - current_line) : strlen(current_line);

	temp_string = malloc(current_line_length+1);
	if(temp_string){
		memcpy(temp_string, current_line, current_line_length);
		temp_string[current_line_length] = '\0';	// Null terminate the string
	}
	else {
		printf("malloc() failed!?\n");
	}
	printf("Current line: %s\n", temp_string);
	parse_current_line(temp_string);
	free(temp_string);
	current_line = next_line ? next_line + 1 : NULL;
	return current_line;
}

void parse_asm_file(const char *asm_file){

	const char * current_line;
	char stop_string[100];

	current_line = asm_file;	// initialize the current line pointer to point at start of asm_file
	if(current_line[0] == '\n'){	// TODO(Marko): Skip the first line if it's just a blank line -- hacky bugfix that should be in preprocess.c
		current_line++;
}
	while(current_line){
		current_line = parse_line_and_update_to_next_line(current_line);
	}
}