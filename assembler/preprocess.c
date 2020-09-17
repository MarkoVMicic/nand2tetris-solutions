#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define VARIABLE_MEMORY_BLOCK_START 5000
#define VARIABLE_MEMORY_BLOCK_END 15000


char * remove_whitespace(char *asm_string)
{
	int count;
	int i;
	int string_length = strlen(asm_string);

	count = 0;
	for(i = 0; i < string_length; i++)	
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
	int string_length = strlen(asm_string);

	count = 0;
	for(i = 0; i < string_length; i++)	
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
	int string_length = strlen(asm_string);

	count = 0;
	for(i = 0; i < string_length; i++)
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
	int string_length = strlen(asm_string);

	count = 0;
	for(i = 0; i < string_length; i++)
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

	int string_length = strlen(asm_string);
	for(i = 0; i < string_length; i++)
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
					// TODO(Marko): Right now I'm just filling every commented char with a space. I would
					// 				like to find a more elegant solution for this, even though I call a 
					//				function after this that removes spaces.  
					asm_string[i+k] = ' ';
				}
			}
		}
	}
	return asm_string;
}


char * remove_labels(char * asm_string)
{
	// Remove labels once we put them into variable table. 
	int i;
	int j;
	int k;

	int string_length = strlen(asm_string);
	for(i=0; i < string_length; i++)
	{
		if(asm_string[i] == '(')
		{
			j = i;
			// TODO(Marko): should probably check for end of string too 
			while(asm_string[j] != '\n')
			{
				j++;
			}
			for(k=0; k < j-i; k++)
			{
				// TODO(Marko): Right now I'm just filling every commented char with a space. I would
				// 				like to find a more elegant solution for this, even though I call a 
				//				function after this that removes spaces.  
				asm_string[i+k] = ' ';
			}
		}
	}
	return asm_string;
}


void insert_predefined_symbols_into_variable_table(linked_list ** head)
{

	append_entry_to_end_of_list(head, "R0", (unsigned short) 0);
	append_entry_to_end_of_list(head, "R1", (unsigned short) 1);
	append_entry_to_end_of_list(head, "R2", (unsigned short) 2);
	append_entry_to_end_of_list(head, "R3", (unsigned short) 3);
	append_entry_to_end_of_list(head, "R4", (unsigned short) 4);
	append_entry_to_end_of_list(head, "R5", (unsigned short) 5);
	append_entry_to_end_of_list(head, "R6", (unsigned short) 6);
	append_entry_to_end_of_list(head, "R7", (unsigned short) 7);
	append_entry_to_end_of_list(head, "R8", (unsigned short) 8);
	append_entry_to_end_of_list(head, "R9", (unsigned short) 9);
	append_entry_to_end_of_list(head, "R10", (unsigned short) 10);
	append_entry_to_end_of_list(head, "R11", (unsigned short) 11);
	append_entry_to_end_of_list(head, "R12", (unsigned short) 12);
	append_entry_to_end_of_list(head, "R13", (unsigned short) 13);
	append_entry_to_end_of_list(head, "R14", (unsigned short) 14);
	append_entry_to_end_of_list(head, "R15", (unsigned short) 15);

	append_entry_to_end_of_list(head, "SCREEN", (unsigned short) 16384);
	append_entry_to_end_of_list(head, "KBD", (unsigned short) 24576);
	append_entry_to_end_of_list(head, "SP", (unsigned short) 0);
	append_entry_to_end_of_list(head, "LCL", (unsigned short) 1);
	append_entry_to_end_of_list(head, "ARG", (unsigned short) 2);
	append_entry_to_end_of_list(head, "THIS", (unsigned short) 3);
	append_entry_to_end_of_list(head, "THAT", (unsigned short) 4);
}


void insert_labels_into_variable_table(char * asm_string, linked_list **head)
{
	// Pass through asm_string, collecting location symbols in 
	// parenthesis. Keep track of line count, do not count lines 
	// where the labels are. No duplicates are allowed. 
	int line_count = 1;
	int current_line_length;
	int label_string_length;
	char * next_line;
	char * current_line = asm_string;
	char * temp_string;
	char * label_string; 
	char * end_of_label_string;
	// Look for the next-line position.
	while(current_line) 
	{	
		next_line = strchr(current_line, '\n');
		current_line_length = next_line ? (next_line - current_line) : strlen(current_line);

		temp_string = malloc(current_line_length+1);
		if(temp_string == NULL)
		{
			printf("malloc() failed.\n");
		}
		memcpy(temp_string, current_line, current_line_length);
		temp_string[current_line_length] = '\0';
		if(temp_string[0] == '(')
		{
			end_of_label_string = strchr(temp_string, ')');
			label_string_length = end_of_label_string - temp_string;
			label_string = malloc(label_string_length);
			memcpy(label_string, temp_string+1, label_string_length);
			label_string[label_string_length-1] = '\0';
			append_entry_to_end_of_list(head, label_string, (unsigned short)line_count);
			free(label_string);
		}
		else
		{
			// Increment line count when you aren't processing the bracketed label.
			line_count++;
		}
		free(temp_string);
		current_line = next_line ? next_line + 1 : NULL;
	}
}
char * preprocess_symbols(char *asm_string)
{
	/*
		3 types of symbols:
			1. Predefined symbols
			2. Labels (things that appear in brackets)
			3. variables. 
		All are handled the same way: put in linked list with associated address, then go through
		the string and replace the variables with the address. 

		Notes:
			• The addresses of labels are guaranteed not to collide with each other since they can 
			  only occupy one line. 
			• It does not matter if the addresses of labels collide with the addresses of  
			  predefined symbols or with the addresses of variables because labels correspond to 
			  ROM addresses, while the other two correspond to RAM addresses. 
			• To prevent RAM collisions, we define some VARIABLE_MEMORY_BLOCK_START and 
			  VARIABLE_MEMORY_BLOCK_END range to store variables in some defined place in memory. 
			  For now, I'm going to assume that a programmer is not going to use more than 10000
			  variables in their code. I'll allocate registers 5000 - 15000. 
	*/
	linked_list * variable_table; 
	initialize_linked_list(&variable_table);
	insert_predefined_symbols_into_variable_table(&variable_table);

	insert_labels_into_variable_table(asm_string, &variable_table);
	// Get rid of extraneous head part. 
	remove_entry_from_beginning_of_list(&variable_table);
	asm_string = remove_labels(asm_string);
	asm_string = remove_whitespace(asm_string);
	asm_string = remove_blank_lines(asm_string);

	insert_variables_into_variable_table(asm_string, &variable_table);

	asm_string = replace_symbols_with_addresses(asm_string, &variable_table);

	delete_linked_list(&variable_table);
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
		asm_string = malloc(length+1);		// allocate the asm_string -- remember to free this when done! 
		if (asm_string)
		  {
		    fread(asm_string, 1, length, file);
		  }
		  fclose(file);
		asm_string[length] = '\0';
	}
	else
	{
		asm_string = malloc(1);
		asm_string[0] = '\0';
		printf("something went wrong.\n");
	}
	return asm_string;
}