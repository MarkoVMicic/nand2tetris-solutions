#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

#define VARIABLE_MEMORY_BLOCK_START 1024
#define VARIABLE_MEMORY_BLOCK_END 15000

#define NUMERIC_STRING "0123456789"

// NOTE(Marko): if we were dealing with truly massive programs, instead of
// 				trying to maintain the string on the stack, we could just
//				read and write line-by-line into an intermediate file, 
//				overwriting that file each time.  


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
					// TODO(Marko): Right now I'm just filling every commented 
					//				char with a space. I would like to find a 
					// 				more elegant solution for this, even 
					//				though I call a function after this that 
					// 				removes spaces.  
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
				// TODO(Marko): Right now I'm just filling every commented 
				//				char with a space. I would like to find a more 
				//			    elegant solution for this, even though I call 
				//				a function after this that removes spaces.  
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
	int line_count = 0;
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
		current_line_length = next_line ? 
			(next_line - current_line) : strlen(current_line);

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
			if(string_is_in_list(*head, label_string) == 1)
			{
				// TODO(Marko): Error handling -- for now we just print a 
				//				statement, and still add the value to the list 
				//				at the end. The resultant behavior is to just 
				// 				default to using the first value in the table
				//				when preprocessing the @<label_string> 
				// 				statement, which could result in bad behavior 
				//				in the program -- ideally we want to abort
				//				the assembly process instead of just warning 
				//				the programmer that they've made a mistake. 
				printf("Error: label (%s) is already in the table. \n",
				 	label_string);
				printf("Either you have tried to use a predefined symbol as"); 
				printf("a lable, or you have a duplicate label (the same ");
				printf("label cannot demarcate multiple places in the ");
				printf("program).\n");

			}
			append_entry_to_end_of_list(head, label_string, 
										(unsigned short)line_count);
			free(label_string);
		}
		else
		{
			// Increment line count when you aren't processing the bracketed 
			// label.
			line_count++;
		}
		free(temp_string);
		current_line = next_line ? next_line + 1 : NULL;
	}
}

void insert_variables_into_variable_table(char * asm_string, 
										  linked_list **head)
{
	int variable_address = VARIABLE_MEMORY_BLOCK_START;
	int allowed_variable_count; 
	int current_line_length;

	char * next_line;
	char * current_line = asm_string;
	char * temp_string;
	char * variable_string = NULL; 
	char * end_of_variable_string;

	allowed_variable_count = VARIABLE_MEMORY_BLOCK_END - 
							 VARIABLE_MEMORY_BLOCK_START;
	/*
		let + denote the newline char '\n', and • denote the null-terminator char '\0'. Then we have this setup: 

						current_line_length
					 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
					{							  }
		asm_string: @123456789098765432123456789098+abcdef...
					^							   ^
					|							   |
					|							   |
				current_line 				   next_line

		temp_line will contain everything from current_line pointer to 
		next_line pointer without the \n character. This means we cannot
		just use strchr(temp_string, '\n'); to find the end of temp_string
		the way we have done in other functions. Note the structure of 
		temp_string, given to us because we allocate using 
		malloc(current_line_length+1). Suppose we use • to denote the null-terminate string '\0', then we get the following result:

		temp_string = malloc(current_line_length+1); // allocating 32 bytes in this e.g.

						    current_line_length = 31
						  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
						 {							   }
			temp_string: @*******************************
														^
														|
													+1 byte here
		// copying first 31 bytes											
				memcpy(temp_string, current_line, current_line_length); 
							 current_line_length = 31
							  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
							 {							   }
				asm_string:  @123456789012345678901234567890+abcdef...
							 |||||||||||||||||||||||||||||||
							 |||||||||||||||||||||||||||||||
							 |||||||||||||||||||||||||||||||
				temp_string: @123456789012345678901234567890*	32 bytes
							 {							   }
							  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
						     current_line_length = 31

		temp_string[current_line_length] = '\0';	   // setting 32nd byte to •

						     current_line_length = 31
						  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
						 {							   }
			temp_string: @123456789012345678901234567890•	32 bytes

		we want to retrieve everything after the @, so we can just:

		char * var_string = malloc(current_line_length); // allocating 31 bytes

    					      current_line_length
    					  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
    					 {							   }
			temp_string: @123456789012345678901234567890•	32 bytes

			var_string:  *******************************	31 bytes
     					 {							   }
     					  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
							current_line_length							

		memcpy(var_string, temp_string+1, current_line_length-1); // copying 30 bytes
    					      current_line_length
    					  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
    					 {							   }
			temp_string: @123456789012345678901234567890•	32 bytes
						  ||||||||||||||||||||||||||||||
						 //////////////////////////////
						 ||||||||||||||||||||||||||||||
			var_string:  123456789012345678901234567890*	31 bytes
     					 {							   }
     					  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
							current_line_length							

		var_string[current_line_length-1] = '\0' // Setting 31st byte to •
						 
			var_string:  123456789012345678901234567890•	31 bytes
     					 {							   }
     					  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
							current_line_length							


	*/
	while(current_line)
	{
		next_line = strchr(current_line, '\n');
		current_line_length = next_line ? 
			(next_line - current_line) : strlen(current_line);

		if(current_line[0] == '@' && (
			strchr(NUMERIC_STRING, current_line[1]) == NULL))
		{
			temp_string = malloc(current_line_length+1);
			if(temp_string == NULL)
			{
				printf("malloc() failed.\n");
			}
			memcpy(temp_string, current_line, current_line_length);
			temp_string[current_line_length] = '\0';
			variable_string = malloc(current_line_length);
			memcpy(variable_string, temp_string+1, current_line_length-1);
			variable_string[current_line_length-1] = '\0';	
			if((string_is_in_list(*head, variable_string) == 0) &&
				(variable_address <= VARIABLE_MEMORY_BLOCK_END))
			{
				append_entry_to_end_of_list(head, variable_string, 
											variable_address);
				variable_address++;
			}
			free(variable_string);
			free(temp_string);
		}
		current_line = next_line ? next_line + 1 : NULL;
	}
}


int count_lines_in_string(char * string)
{
	int line_count = 0;
	char * current_line = string;
	char * next_line;
	while(current_line)
	{
		next_line = strchr(current_line, '\n');
		current_line = next_line ? next_line + 1 : NULL;
		line_count++;

	}
	return(line_count);
}


char * append_string_to_string(char * destination_string, char * source_string)
{
	/*
		WARNINGS: 
		(1) Make sure that destination_string has enough space allocated 
		    to it after the null-terminator so that it can contain the
		    contents of source_string, otherwise. you will start writing into unallocated memory which causes undefined behavior or segfault. 
		(2) Make sure that both strings are actually null-terminated.
		(3) Make sure that the pointer you pass to this function for both 
		    strings are copies of the original string pointers, otherwise you
		    will modify the original pointers causing weird behavior.  
	*/
	// Get to the end of the destination_string. 
	while((*destination_string))
	{
		destination_string++;
	}
	// Copy contents of source into destination until you read a null 
	// terminator
	while((*destination_string++ = *source_string++)); 

	// return a pointer to the end of the destination string -- ready for more 
	// appending!
	return(destination_string-1);
}


// TODO(Marko): Possible issue with types int vs unsigned short. Investigate
char * convert_int_to_string(int address)
{
	// NOTE(Marko): I'm sneaking in a newline char here coz I need to. 
	int string_length = snprintf(NULL, 0, "%d", address);
	char * string = malloc(string_length+2);
	snprintf(string, string_length+2, "%d", address);
	string[string_length] = '\n';
	string[string_length+1] = '\0';
	return string;
}


char * replace_symbols_with_addresses(char * asm_string, linked_list **head)
{
	/*
		At this stage, every asm_string that gets to here (that is valid) will 
		consist of some number of lines. Our goal is to replace A-instruction 
		lines (i.e. lines beginning with @) which contain a variable (i.e. not 
		an explicit numerical address) with the corresponding numerical 
		address found in our variable table. 

		Since we can't realistically modify the asm_string in-place, we will 
		instead create a new string and copy over the contents of the old 
		string, replacing variables with the addresses as we go. 

		The question then becomes: how much space do we need to allocate for 
		this new string? 

		The efficient way to do this is to first check the line-by-line 
		character counts, substituting the character count of addresses 
		wherever strings are, keeping track of the new count, then allocate 
		the appropriate amount of memory. 

		The easier way is to allocate for the worst possible case, where we 
		write the largest possible numeral in place of the variable, and every 
		A-instruction consists of a variable. In this case, we observe that 
		the maximum number of unsigned short is 65535, which, in character 
		form, is 5 bytes. Thus the largest string for an A-instruction will be 
		6 bytes. For C-instructions, the worst possible case is a string that 
		looks like AMD=D+A;JMP\n  which is 12 bytes. So the assumption is 
		simply that we have a program full of the longest possible D 
		instruction repeated on every line, line_count*12, add 1 byte for the 
		null-terminator. 
	*/
	int line_count;
	int max_line_length = 12;
	int modified_asm_string_max_length;
	int modified_asm_string_length;
	int current_line_length;
	unsigned short address;


	char * modified_asm_string;
	char * ptr_mod_string;
	char * current_line;
	char * next_line; 
	char * temp_string;
	char * variable_string;
	char * address_string; 

	line_count = count_lines_in_string(asm_string);
	// Remove blank line at end of program from line count. 
	if(line_count % 2)
	{
		line_count--;
	}
	
	modified_asm_string_max_length = line_count * max_line_length;

	modified_asm_string = malloc(modified_asm_string_max_length + 1);
	modified_asm_string[0] = '\0';
	ptr_mod_string = modified_asm_string;

	current_line = asm_string;
	int i=1;
	while(current_line)
	{
		next_line = strchr(current_line, '\n');
		current_line_length = next_line ? 
			(next_line - current_line) : strlen(current_line);
		temp_string = malloc(current_line_length+2);
		if(current_line[0] == '@' && 
			(strchr(NUMERIC_STRING, current_line[1]) == 0))
		{
			// Replace variable with address in variable table
			// TODO(Marko): Implement 
			memcpy(temp_string, current_line, current_line_length);
			temp_string[current_line_length] = '\0';
			variable_string = malloc(current_line_length);
			memcpy(variable_string, temp_string+1, current_line_length-1);
			variable_string[current_line_length-1] = '\0';
			address = retrieve_address_from_string_in_list(head, 
                                         variable_string);
			address_string = convert_int_to_string(address);
			// NOTE(Marko): This is not strictly necessary -- the parser can 
			//				probably handle not having an @ at the beginning
			//				of A-instructions, but for the sake of writing
			//				clearer code, I've stuck it back in there. 
			//				There's probably a better way to go about making
			//				sure it stays there, but thanks to the negigible 
			//				overhead of append_string_to_string(), this isn't
			//				a large issue. 
			ptr_mod_string = append_string_to_string(ptr_mod_string,
													 "@");
			ptr_mod_string = append_string_to_string(ptr_mod_string,
													 address_string);
			free(variable_string);
			free(address_string); 
		}
		else
		{
			// Copy the string over as is. 
			memcpy(temp_string, current_line, current_line_length);
			temp_string[current_line_length] = '\n';
			temp_string[current_line_length+1] = '\0';
			ptr_mod_string = append_string_to_string(ptr_mod_string,
													 temp_string);
		}

		free(temp_string);
		current_line = next_line ? next_line + 1 : NULL;
		i++;
	}
	// NOTE(Marko): Since I allocated asm_string in assembler.c main(), then 
	// 				called preprocess.c process_asm_string() which returns the
	//				same char *asm_string, and then free asm_string later in 
	//				assembler.c main(), I actually need to hold onto the 
	//				pointer, which means I can't simply return 
	//				modified_asm_string without causing myself a bunch of 
	//				memory-management headaches (I'd need to not return the 
	//				same asm_string, but somehow trust that it would get 
	//				deallocated later on over here, and that somehow I'd find 
	//				a good place to deallocate modified_asm_string later on 
	//				too. That's why I call realloc() on asm_string and perform 
	//				this seemingly redundant copy-back of modified_asm_string. 
	//				One way around this would be to just refactor the code so 
	//				that you are constantly writing to a temporary file for 
	//				each step of the preprocessing, instead of maintaining a 
	//				string in memory. 
	// TODO(Marko): This may or may not allocate 1 more byte than is actually 
	//				needed. 
	modified_asm_string_length = strlen(modified_asm_string);
	asm_string = realloc(asm_string, modified_asm_string_length + 1);
	memcpy(asm_string, modified_asm_string, modified_asm_string_length);
	asm_string[modified_asm_string_length] = '\0';
	free(modified_asm_string);

	return asm_string;
}

char * preprocess_symbols(char *asm_string)
{
	/*
		3 types of symbols:
			1. Predefined symbols
			2. Labels (things that appear in brackets)
			3. variables. 
		All are handled the same way: put in linked list with associated 
		address, then go through the string and replace the variables with the 
		address. 

		Notes:
			• The addresses of labels are guaranteed not to collide with each 
			  other since they can only occupy one line. 
			• It does not matter if the addresses of labels collide with the 
			  addresses of predefined symbols or with the addresses of 
			  variables because labels correspond to ROM addresses, while the 
			  other two correspond to RAM addresses. 
			• To prevent RAM collisions, we define some 
			  VARIABLE_MEMORY_BLOCK_START and VARIABLE_MEMORY_BLOCK_END range 
			  to store variables in some defined place in memory. As specified
			  in the textbook, I'll start allocating from 1024. 
	*/
	linked_list * variable_table; 
	initialize_linked_list(&variable_table);
	insert_predefined_symbols_into_variable_table(&variable_table);

	insert_labels_into_variable_table(asm_string, &variable_table);
	insert_variables_into_variable_table(asm_string, &variable_table);
	// Get rid of extraneous head part. 
	remove_entry_from_beginning_of_list(&variable_table);
	// puts("Variable table:\n");
	// print_linked_list(variable_table);

	asm_string = remove_labels(asm_string);
	asm_string = remove_whitespace(asm_string);
	asm_string = remove_blank_lines(asm_string);

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
		// allocate the asm_string -- remember to free this when done! 
		asm_string = malloc(length+1);		
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
