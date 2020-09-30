#include "linked_list.h"
#ifndef PREPROCESS_H
#define PREPROCESS_H

char * remove_whitespace(char *asm_string);

char * remove_carriage_returns(char *asm_string);

char * remove_tabs(char *asm_string);

char * remove_consecutive_newlines(char *asm_string);

char * remove_blank_lines(char *asm_string);

char * remove_comments(char *asm_string);

char * remove_labels(char * asm_string);

void insert_predefined_symbols_into_variable_table(linked_list ** head);

void insert_labels_into_variable_table(char * asm_string, linked_list **head);

void insert_variables_into_variable_table(char * asm_string, 
                                          linked_list **head);

int count_lines_in_string(char * string);

char * append_string_to_string(char * destination_string, 
                               char * source_string);

char * convert_int_to_string(int address);

char * replace_symbols_with_addresses(char * asm_string, linked_list **head);

char * preprocess_symbols(char *asm_string);

char * process_asm_string(char *asm_string);

char * open_file_store_as_string(char *file_path);

#endif
