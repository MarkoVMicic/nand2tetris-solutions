#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

void insert_predefined_symbols_into_variable_table(linked_list * head)
{

	append_entry_to_end_of_list(head, "R0", 0);
	append_entry_to_end_of_list(head, "R1", 1);
	append_entry_to_end_of_list(head, "R2", 2);
	append_entry_to_end_of_list(head, "R3", 3);
	append_entry_to_end_of_list(head, "R4", 4);
	append_entry_to_end_of_list(head, "R5", 5);
	append_entry_to_end_of_list(head, "R6", 6);
	append_entry_to_end_of_list(head, "R7", 7);
	append_entry_to_end_of_list(head, "R8", 8);
	append_entry_to_end_of_list(head, "R9", 9);
	append_entry_to_end_of_list(head, "R10", 10);
	append_entry_to_end_of_list(head, "R11", 11);
	append_entry_to_end_of_list(head, "R12", 12);
	append_entry_to_end_of_list(head, "R13", 13);
	append_entry_to_end_of_list(head, "R14", 14);
	append_entry_to_end_of_list(head, "R15", 15);

	append_entry_to_end_of_list(head, "SCREEN", 16384);
	append_entry_to_end_of_list(head, "KBD", 24576);
	append_entry_to_end_of_list(head, "SP", 0);
	append_entry_to_end_of_list(head, "LCL", 1);
	append_entry_to_end_of_list(head, "ARG", 2);
	append_entry_to_end_of_list(head, "THIS", 3);
	append_entry_to_end_of_list(head, "THAT", 4);
}

char * preprocess_symbols(char *asm_string)
{
	linked_list * variable_table; 
	variable_table = initialize_linked_list(variable_table);
	// initialize variable_table with predefined symbols
	insert_predefined_symbols_into_variable_table(variable_table);
	// remove initializing head value
	remove_entry_from_beginning_of_list(&variable_table);
	print_linked_list(variable_table);
	return asm_string;
}


char * remove_whitespace(char *asm_string)
{
	int count;
	int i;


	count = 0;
	for(i = 0; asm_string[i]; i++)
	for(i = 0; asm_string[i] != '\0'; i++)
	{
		if(asm_string[i] != ' ')
		{
			asm_string[count++] = asm_string[i]; 
		}
	}
	asm_string[count] = '\0'; // Null terminate the string
	return asm_string;

}


char * remove_carriage_returns(char *asm_string)
{
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++)
	{
		if(asm_string[i] != '\r')
		{
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0';
	return asm_string;
}


char * remove_tabs(char *asm_string)
{
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++)
	{
		if(asm_string[i] != 9)
		{
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0';
	return asm_string;
}


char * remove_consecutive_newlines(char *asm_string)
{
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++)
	{
		if((asm_string[i] != '\n') || ((i > 0) && (asm_string[i-1] != '\n')))
		{
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0'; // Null terminate the string
	return asm_string;
}


char * remove_blank_lines(char *asm_string)
{
	asm_string = remove_carriage_returns(asm_string);
	asm_string = remove_consecutive_newlines(asm_string);
	
	return asm_string;
}


char * remove_comments(char *asm_string)
{
	// ASM comments are identical to single-line comments in C. 
	int i;
	int j;
	int k;

	for(i = 0; asm_string[i]; i++)
	{
		if(asm_string[i] == '/')
		{			// First slash
			if(asm_string[i+1] == '/')
			{	// Second slash indicates comment!
				// Ignore until end of line
				j = i;
				while(asm_string[j] != '\n')
				{
					j++;
				}
				for(k=0; k < j-i; k++)
				{
					asm_string[i+k] = ' ';
				}
			}
		}
	}
	return asm_string;
}


char * process_asm_string(char *asm_string)
{
	asm_string = remove_comments(asm_string);
	asm_string = remove_whitespace(asm_string);
	asm_string = remove_tabs(asm_string);
	asm_string = remove_blank_lines(asm_string);
	asm_string = preprocess_symbols(asm_string);
	// TODO: Handle variables here.
	return asm_string;
}


char * open_file_store_as_string(const char *file_path)
{
	char *asm_string;
	long length;
	FILE *file; 

	printf("Parsing file found at %s ...\n", file_path);
	file = fopen(file_path, "r");
	if(file)
	{	// i.e. if file != NULL
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		asm_string = malloc(length);		// allocate the asm_string -- remember to free this when done! 
		if (asm_string)
		  {
		    fread(asm_string, 1, length, file);
		  }
		  fclose(file);
	}
	else
	{
		asm_string = malloc(1);
		asm_string[0] = '\0';
		printf("something went wrong.\n");
	}
	// printf("%s\n", asm_string);
	return asm_string;
}