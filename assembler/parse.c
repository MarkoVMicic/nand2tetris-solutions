#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocess.h"
#include "instruction_table.h"

#define NUM_DEST_INSTRUCTIONS 8
#define NUM_COMP_INSTRUCTIONS 28
#define NUM_JUMP_INSTRUCTIONS 8
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

void parse_c_instruction(char * current_line, 
	                     char * parsed_line,
	                     instruction_table * dest,
	                     instruction_table * comp,
	                     instruction_table * jump)
{
	char * comp_ptr;
	char * jump_ptr;

	char * dest_string;
	char * comp_string;
	char * jump_string;

	char * ptr_parsed_line;

	int dest_string_length;
	int comp_string_length;
	int jump_string_length;
	char * pos_equals_sign;
	char * pos_semi_colon;

	int i;

	pos_equals_sign = strchr(current_line, '=');
	pos_semi_colon = strchr(current_line, ';');
	if(!pos_equals_sign)
	{
		// no equals sign found -- no dest string. 
		dest_string = malloc(1);
		dest_string[0] = '\0';
		dest_string_length = 0;
		// comp_ptr starts at beginning of line
		comp_ptr = current_line; 
	}
	else
	{
		// dest string is everything up to the equals sign; 
		dest_string_length = pos_equals_sign - current_line; 
		dest_string = malloc(dest_string_length+1);
		memcpy(dest_string, current_line, dest_string_length);
		dest_string[dest_string_length] = '\0';
		// comp_ptr starts at equals sign. 
		comp_ptr = pos_equals_sign+1;
	}

	if(!pos_semi_colon)
	{
		// No semi-colon found -- no jump string. 
		jump_string = malloc(1);
		jump_string[0] = '\0';
		jump_string_length = 0;

		// comp string is everything from comp_ptr to the end of the line
		comp_string_length = strlen(comp_ptr);
		comp_string = malloc(comp_string_length+1);
		memcpy(comp_string, comp_ptr, comp_string_length);
		comp_string[comp_string_length] = '\0';
	}
	else
	{
		// comp string goes from comp_ptr to the semi-colon
		comp_string_length = pos_semi_colon - comp_ptr;
		comp_string = malloc(comp_string_length+1);
		memcpy(comp_string, comp_ptr, comp_string_length);
		comp_string[comp_string_length] = '\0';
		// Jump string begins at semi-colon.
		jump_ptr = pos_semi_colon+1;
		jump_string_length = strlen(jump_ptr);
		jump_string = malloc(jump_string_length+1);
		memcpy(jump_string, jump_ptr, jump_string_length);
		jump_string[jump_string_length] = '\0';
	}

	parsed_line[0] = '\0';
	ptr_parsed_line = parsed_line;
	ptr_parsed_line = append_string_to_string(ptr_parsed_line, "111");
	for(i=0; i<NUM_DEST_INSTRUCTIONS; i++)
	{
		if(strcmp(dest_string, dest[i].symbols) == 0)
		{
			ptr_parsed_line = append_string_to_string(ptr_parsed_line, dest[i].binary_address_string);
			break;
		}
	}

	for(i=0; i<NUM_COMP_INSTRUCTIONS; i++)
	{
		if(strcmp(comp_string, comp[i].symbols) == 0)
		{
			ptr_parsed_line = append_string_to_string(ptr_parsed_line, comp[i].binary_address_string);
			break;
		}
	}

	for(i=0; i<NUM_JUMP_INSTRUCTIONS; i++)
	{
		if(strcmp(jump_string, jump[i].symbols) == 0)
		{
			ptr_parsed_line = append_string_to_string(ptr_parsed_line, jump[i].binary_address_string);
		}
	}
	parsed_line[16] = '\n';
	parsed_line[17] = '\0';
	puts(parsed_line);


	free(dest_string);
	free(comp_string);
	free(jump_string);
}


void parse_a_instruction(char * current_line, char * parsed_line)
{
	int machine_code_instruction[16];
	int length_of_current_line = strlen(current_line);
	int i;
	int A_instruction_decimal;

	char * A_instruction_address;
	char * temp_string;
	

	// Initialize machine_code_instruction to all 0s. 
	for(i=0; i<16; i++)
	{
		machine_code_instruction[i] = 0;
	}
	
	A_instruction_address = current_line + 1;
	A_instruction_decimal = atoi(A_instruction_address);
	for(i=0; A_instruction_decimal > 0; i++){

		machine_code_instruction[15-i] = A_instruction_decimal % 2;
		A_instruction_decimal = A_instruction_decimal / 2;
		if(i == 15)
		{
			// Prevent going out of bounds
			printf("The numerical value of the address in decimal is too large ");
			printf("to be converted into a 15 bit integer.");
			break;
		}
	}
	temp_string = malloc(18);
	for(i=0; i<16; i++)
	{
		// The below line is a little bit of an evil hack. Basically, machine_code_instruction[i] is an int, and 
		// so by abusing ASCII, we can add '0' to the int to convert it into its string equivalent. 
		temp_string[i] = machine_code_instruction[i] + '0';

	}
	temp_string[16] = '\n';	// Add newline to the end of the string
	temp_string[17] = '\0';    // Null terminate the string. 
	strcpy(parsed_line, temp_string);
	free(temp_string);
}


void write_parsed_string_to_file(char * parsed_string, char * output_file_path)
{
	FILE * file;
	file = fopen(output_file_path, "a");
    if(file == NULL)
    {
    	// TODO(Marko): Error handling
        printf("\nUnable to open '%s' file.\n", output_file_path);
        printf("Please check whether file exists and you have write privilege.\n");
    }
    fputs(parsed_string, file);
	fclose(file);
}


void parse_asm_string(char *asm_string, char * output_file_path)
{
	int current_line_length;

	char * current_line;
	char * next_line;
	char * temp_string;
	char * parsed_string;

	instruction_table * dest_table;
	instruction_table * comp_table;
	instruction_table * jump_table; 

	dest_table = malloc(sizeof(instruction_table)*NUM_DEST_INSTRUCTIONS);
	comp_table = malloc(sizeof(instruction_table)*NUM_COMP_INSTRUCTIONS);
	jump_table = malloc(sizeof(instruction_table)*NUM_JUMP_INSTRUCTIONS);

	insert_instructions_into_dest_table(dest_table);
	insert_instructions_into_comp_table(comp_table);
	insert_instructions_into_jump_table(jump_table);

	current_line = asm_string;	
	while(current_line)
	{
		next_line = strchr(current_line, '\n');		
		current_line_length = next_line ? 
			(next_line - current_line) : strlen(current_line);
		if(current_line_length)
		{
			temp_string = malloc(current_line_length+1);
			memcpy(temp_string, current_line, current_line_length);
			temp_string[current_line_length] = '\0';
			if(temp_string == NULL)
			{
				printf("malloc() failed.\n");
			}

			// each line is parsed to a 16-bit instruction, a new-line char, and a null-terminator
			parsed_string = malloc(18);
			if(temp_string[0] == '@')
			{
				parse_a_instruction(temp_string, parsed_string);
			}
			else
			{
				parse_c_instruction(temp_string, 
							        parsed_string,
							        dest_table,
							        comp_table,
							        jump_table);
			}
			write_parsed_string_to_file(parsed_string, output_file_path);
			free(temp_string);
			free(parsed_string);
		}
		current_line = next_line ? next_line + 1 : NULL;
	}
	free(dest_table);
	free(comp_table);
	free(jump_table);
}
