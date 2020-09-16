#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAD_STRING "head of linked list"

typedef struct linked_list
{
    char * string;
    unsigned short address;
    struct linked_list * next;
} linked_list;


linked_list * initialize_linked_list(linked_list * head)
{
    head = malloc(sizeof(linked_list));
    if(head == NULL)
    {
        printf("malloc() failed. \n");
    }
    head->string = malloc(strlen(HEAD_STRING)+1);
    if(head->string == NULL)
    {
        printf("malloc() failed.\n");
    }
    strcpy(head->string, HEAD_STRING);
    head->string[strlen(HEAD_STRING)] = '\0';
    head->address = 0;
    head->next = NULL;
    return head;
}


void print_linked_list(linked_list * head)
{
    if(head == NULL)
    {
        printf("Error: no list available.\n");
        return;
    }
    else
    {
        linked_list * current = head;
        while(current != NULL)
        {   
            printf("%s, %d\n", current->string, current->address);
            current = current->next;
        }
        printf("\n");
    }
}


void append_entry_to_end_of_list(linked_list * head, char * input_string, int address)
{
    linked_list * new_entry;
    new_entry = malloc(sizeof(linked_list));
    if(new_entry == NULL)
    {
        printf("malloc() failed.\n");
    }
    linked_list * current = head;

    new_entry->string = malloc(strlen(input_string)+1);
    if(new_entry->string == NULL)
    {
        printf("malloc() failed.\n");
    }
    strcpy(new_entry->string, input_string);
    new_entry->string[strlen(input_string)] = '\0';

    new_entry->address = address; 
    // Find last node
    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_entry;   
}


void add_entry_as_new_head_of_list(linked_list **head, char * input_string, int address)
{
    linked_list * new_head;
    new_head = malloc(sizeof(linked_list));
    if(new_head == NULL)
    {
        printf("malloc() failed.\n");
    }
    new_head->string = malloc(strlen(input_string)+1);
    if(new_head->string == NULL)
    {
        printf("malloc() failed.\n");
    }
    strcpy(new_head->string, input_string);
    new_head->string[strlen(input_string)] = '\0';
    new_head->address = address;
    new_head->next = *head;
    *head = new_head;
}


void remove_entry_from_beginning_of_list(linked_list **head)
{
    linked_list * next_entry = NULL;
    // store next node
    next_entry = (*head)->next;
    // remove current head node
    free((*head)->string);
    free(*head);
    // set head node to next node. 
    *head = next_entry;
}



void remove_entry_from_end_of_list(linked_list **head)
{
    // If there is only one item on the list, remove it. 

    if((*head)->next == NULL)
    {
        free((*head)->string);
        free(*head);
        *head = NULL;
        return;
    }
    else
    {// Otherwise, get to the second-last value on the list:
        linked_list * current = *head; 
        while(current->next->next != NULL)
        {
            current = current->next;
        }
        free(current->next->string);
        free(current->next);
        current->next = NULL;
    }
}


void remove_entry_by_index(linked_list **head, int index)
{
    int i;
    linked_list *current = *head;
    linked_list *temp_entry = NULL;

    if(index==0)
    {
        remove_entry_from_beginning_of_list(head);
        // NOTE(Marko): Should this function be int that returns an error-code?
        return;
    }

    // Get to the entry just before the one we wish to remove. 
    for(i=0; i<index-1; i++)
    {
        if(current->next == NULL) // Already reached end of the list
        {
            printf("Error: Index out of bounds of linked list. \n");
            // NOTE(Marko): Should this function be int that returns an error-code? 
            return; 
        }
        current = current->next; 
    }

    temp_entry = current->next;
    current->next = temp_entry->next; 
    free(temp_entry->string);
    free(temp_entry);
}


void remove_entry_by_address(linked_list **head, int input_address)
{
    linked_list *current = *head;
    linked_list *temp_entry = NULL;
    if((*head)->address == input_address)
    {
        remove_entry_from_beginning_of_list(head);
        return;
    }

    while(current->next != NULL)
    {
        if(current->next->address == input_address)
        {
            break;
        }
    }
    temp_entry = current->next;
    if(temp_entry == NULL)
    {
        printf("Error: Entry with address %d not found in linked list.\n", input_address);
        return;
    }
    current->next = temp_entry->next; 
    free(temp_entry->string);
    free(temp_entry);
}


void remove_entry_by_string(linked_list **head, char * input_string)
{
    linked_list *current = *head;
    linked_list *temp_entry = NULL;
    if(strcmp((*head)->string, input_string) == 0)
    {
        remove_entry_from_beginning_of_list(head);
        return;
    }

    while(current->next != NULL)
    {
        if(strcmp(current->next->string, input_string) == 0)
        {
            break;
        }
    }
    temp_entry = current->next;
    if(temp_entry == NULL)
    {
        printf("Error: Entry with string %s not found in linked list.\n", input_string);
        return;
    }
    current->next = temp_entry->next; 
    free(temp_entry->string);
    free(temp_entry);

}


void delete_linked_list(linked_list **head)
{
    while(*head != NULL)
    {
        remove_entry_from_beginning_of_list(head);
    }

}

// NOTE(Marko): Returning int because we are using negative numbers as error codes. 
int retrieve_address_from_string_in_list(linked_list * head, char * input_string)
{
    if(head == NULL)
    {
        printf("Error: Empty linked list.\n");
        return(-1);
    }
    linked_list * current = head;
    while(current != NULL)
    {   
        if(strcmp(current->string, input_string) == 0)
        {
            break;
        }
        current = current->next;
    }
    if(current == NULL)
    {
        printf("Error: entry with string %s not found in linked list.\n", input_string);
        return(-2);
    }
    return (int) current->address;
}


int string_is_in_list(linked_list * head, char * input_string)
{
    if(head == NULL)
    {
        printf("Error: Empty linked list.\n");
        return(-1);
    }
    linked_list * current = head;
    while(current != NULL)
    {   
        if(strcmp(current->string, input_string) == 0)
        {
            break;
        }
        current = current->next;
    }
    if(current == NULL)
    {   
        // String not found
        return(0);
    }
    else
    {
        // string found. 
        return(1);
    }
}
