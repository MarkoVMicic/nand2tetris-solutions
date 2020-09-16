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
    printf("%d\n", (int) head);
    printf("%d\n", (int)(*head));
    // set head node to next node. 
    *head = next_entry;
    printf("%d\n", (int) head);
    printf("%d\n", (int)(*head));
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


linked_list * retrieve_entry_by_address(linked_list * head, int address)
{
    linked_list * retrieved_entry = NULL;
    linked_list *current = head; 
    while(current->next != NULL)
    {
        if(current->address == address)
        {
            break;
        }
        current = current->next;
    }
    return(current);
}



// For testing.
int main(int argc, char **argv)
{
    char * test_string1 = "test string 1";
    int test_address1 = 12345;

    char * test_string2 = "test string 2";
    int test_address2 = 54321;

    char * test_string3 = "test string 3";
    int test_address3 = 123;

    char * test_string4 = "test string 4";
    int test_address4 = 321;

    char * test_string5 = "test string 5";
    int test_address5 = 1;

    linked_list * table;
    table = initialize_linked_list(table);
    puts("Table initialized."); 
    print_linked_list(table);

    append_entry_to_end_of_list(table, test_string1, test_address1);
    puts("Entry appended.");
    print_linked_list(table);

    append_entry_to_end_of_list(table, test_string2, test_address2);
    puts("Entry appended.");
    print_linked_list(table);

    append_entry_to_end_of_list(table, test_string3, test_address3);
    puts("Entry appended.");
    print_linked_list(table);

    add_entry_as_new_head_of_list(&table, test_string4, test_address4);
    puts("Entry added as new head.");
    print_linked_list(table);

    add_entry_as_new_head_of_list(&table, test_string5, test_address5);
    puts("Entry added as new head.");
    print_linked_list(table);

    return(0);
}