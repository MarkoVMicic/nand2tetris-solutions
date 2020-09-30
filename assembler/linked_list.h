#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list
{
    char * string;
    unsigned short address;
    struct linked_list * next;
} linked_list;


void initialize_linked_list(linked_list **head);

void print_linked_list(linked_list * head);

void append_entry_to_end_of_list(linked_list **head, char * input_string, 
                                 unsigned short address);

void add_entry_as_new_head_of_list(linked_list **head, char * input_string,
                                 unsigned short address); 

void remove_entry_from_beginning_of_list(linked_list **head);

void remove_entry_from_end_of_list(linked_list **head);

void remove_entry_by_index(linked_list **head, int index);

void remove_entry_by_address(linked_list **head, unsigned short input_address);

void remove_entry_by_string(linked_list **head, char * input_string);

void delete_linked_list(linked_list **head);

int retrieve_address_from_string_in_list(linked_list ** head, 
                                         char * input_string);

int string_is_in_list(linked_list * head, char * input_string);

#endif
