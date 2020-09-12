#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * preprocess_variables(char *asm_string){
	// TODO(Marko): Implement
	/*
		I think the best way to implement this preprocessing of variables is to build a
		hash table which uses some hash function to convert a variable length string into
		a definitive address. 


	*/
	return asm_string;
}

char * preprocess_predefined_variables(char *asm_string){

	return asm_string;
}

char * preprocess_symbols(char *asm_string){
	// TODO(Marko): Implement
	/*
		The goal of this function is to pass over the asm string (which, by this point, 
		consists of a single instruction per line with no comments), and to process 
		the two types of symbols: variables and labels. We want to essentially replace
		these variables and symbols with appropriate values.

		We also want to process special values known as pre-defined symbols
	*/
	return asm_string;

}

char * remove_whitespace(char *asm_string){
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++){
		if(asm_string[i] != ' '){
			asm_string[count++] = asm_string[i]; 
		}
	}
	asm_string[count] = '\0'; // Null terminate the string
	return asm_string;

}

char * remove_carriage_returns(char *asm_string){
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++){
		if(asm_string[i] != '\r'){
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0';
	return asm_string;
}

char * remove_tabs(char *asm_string){
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++){
		if(asm_string[i] != 9){
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0';
	return asm_string;
}

char * remove_consecutive_newlines(char *asm_string){
	int count;
	int i;

	count = 0;
	for(i = 0; asm_string[i]; i++){
		if((asm_string[i] != '\n') || ((i > 0) && (asm_string[i-1] != '\n'))){
			asm_string[count++] = asm_string[i];
		}
	}
	asm_string[count] = '\0'; // Null terminate the string
	return asm_string;
}

char * remove_blank_lines(char *asm_string){
	asm_string = remove_carriage_returns(asm_string);
	asm_string = remove_consecutive_newlines(asm_string);
	
	return asm_string;
}

char * remove_comments(char *asm_string){
	// ASM comments are identical to single-line comments in C. 
	int i;
	int j;
	int k;
	for(i = 0; asm_string[i]; i++){
		if(asm_string[i] == '/'){			// First slash
			if(asm_string[i+1] == '/'){	// Second slash indicates comment!
				// Ignore until end of line
				j = i;
				while(asm_string[j] != '\n'){
					j++;
				}
				for(k=0; k < j-i; k++){
					asm_string[i+k] = ' ';
				}
			}
		}
	}
	asm_string = remove_whitespace(asm_string);
	asm_string = remove_blank_lines(asm_string);
	return asm_string;
}

char * process_asm_string(char *asm_string){
	asm_string = remove_whitespace(asm_string);
	asm_string = remove_blank_lines(asm_string);
	asm_string = remove_tabs(asm_string);
	asm_string = remove_comments(asm_string);
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
	if(file){	// i.e. if file != NULL
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
	else{
		asm_string = malloc(1);
		asm_string[0] = '\0';
	}
	// printf("%s\n", asm_string);
	return asm_string;
}