#include "labelsfunctions.h"

Label *init_label_table()
{
    Label *head = (Label *)malloc(sizeof(Label)); /*Allocate memory for the head of the label table*/
    if (!head)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    head->name = NULL;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

ExternLabel *init_extern_label_table()
{
    ExternLabel *head = (ExternLabel *)malloc(sizeof(ExternLabel)); /*Allocate memory for the head of the extern label table*/
    if (!head)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    head->label = NULL;
    head->usage_address = 0;
    head->next = NULL;
    head->prev = NULL;

    return head;
}

int add_extern_label(ExternLabel *head, Label *label, int address)
{
    ExternLabel *newLabel = NULL;
    newLabel = (ExternLabel *)malloc(sizeof(ExternLabel)); /*Allocate memory for the new extern label*/
    if (!newLabel)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return ERROR;
    }

    if (!label || head == NULL)
    {
        return ERROR_NULL_PARAM;
    }

    newLabel->label = label;           /*Set the label of the extern label*/
    newLabel->usage_address = address; /*Set the usage address of the extern label*/

    /*Add the new extern label to the list*/
    newLabel->next = head->next;
    newLabel->prev = head;
    if (head->next != NULL)
    {
        head->next->prev = newLabel;
    }
    head->next = newLabel;
    return 0;
}

Label *find_label(Label *head, char *name)
{
    Label *temp = head;
    if (name == NULL)
    {
        return NULL;
    }
    /*Search for label by name and return pointer*/
    while (temp != NULL)
    {
        if ((temp->name != NULL) && (strcmp(temp->name, name) == 0))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int add_extern_label_to_list(char *line, Label *head)
{
    /*Add extern label to the list with value 0*/
    char *word;
    word = strtok(line, " \t\n");
    word = strtok(NULL, " \t\n");
    add_label(head, word, 0, EXTERNAL);
    return 0;
}

int add_label(Label *head, char *name, int address, LabelType type)
{
    Label *newLabel = NULL;
    if (!head || !name)
        return FAILURE;

    if (find_label(head, name) != NULL) /*Check if label already exists*/
    {
        return ERROR_LABEL_ALREADY_EXISTS;
    }

    newLabel = (Label *)malloc(sizeof(Label)); /*Allocate memory for the new label*/
    if (!newLabel)
    {
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    newLabel->name = (char *)malloc(strlen(name) + 1); /*Allocate memory for the label name*/
    if (!newLabel->name)
    {
        free(newLabel);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    strcpy(newLabel->name, name); /*Copy the label name*/
    newLabel->address = address;
    newLabel->type = type;

    /*Add the new label to the list*/
    newLabel->next = head->next;
    newLabel->prev = head;
    if (head->next != NULL)
    {
        head->next->prev = newLabel;
    }
    head->next = newLabel;
    return SUCCESS;
}

void free_label_table(Label *head)
{
    Label *temp = NULL;
    if (!head)
    {
        return;
    }

    /*Free the label table by looping over it and freeing each*/
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        if (temp->name)
        {
            free(temp->name);
        }
        free(temp);
    }
}

void free_extern_label_table(ExternLabel *head)
{
    ExternLabel *temp = NULL;
    if (!head)
    {
        return;
    }
    /*Free the label table by looping over it and freeing each*/
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int update_data_addresses(Label *head, int IC)
{
    Label *temp = NULL;
    if (!head)
        return FAILURE;

    temp = head->next;

    /*Update the addresses of the data labels by adding IC Final*/
    while (temp != NULL)
    {
        if (temp->type == DATA)
        {
            temp->address += IC;
        }
        temp = temp->next;
    }
    return SUCCESS;
}