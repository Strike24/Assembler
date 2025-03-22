#include "labelsfunctions.h"

Label *init_label_table()
{
    Label *head = (Label *)malloc(sizeof(Label));
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
    ExternLabel *head = (ExternLabel *)malloc(sizeof(ExternLabel));
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
    newLabel = (ExternLabel *)malloc(sizeof(ExternLabel));
    if (!newLabel)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return ERROR;
    }

    if (!label || head == NULL)
    {
        return ERROR_NULL_PARAM;
    }

    newLabel->label = label;
    newLabel->usage_address = address;

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
    char *word;
    word = strtok(line, " \t\n");
    word = strtok(NULL, " \t\n");
    add_label(head, word, 0, EXTERNAL);
    return 0;
}

int add_label(Label *head, char *name, int address, LabelType type)
{
    Label *newLabel;
    if (find_label(head, name) != NULL)
    {
        return ERROR_LABEL_ALREADY_EXISTS;
    }

    newLabel = (Label *)malloc(sizeof(Label));
    if (!newLabel)
    {
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    newLabel->name = (char *)malloc(strlen(name) + 1);
    if (!newLabel->name)
    {
        free(newLabel);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    strcpy(newLabel->name, name);
    newLabel->address = address;
    newLabel->type = type;

    newLabel->next = head->next;
    newLabel->prev = head;
    if (head->next != NULL)
    {
        head->next->prev = newLabel;
    }
    head->next = newLabel;
    return 0;
}

void free_label_table(Label *head)
{
    Label *temp;
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
    ExternLabel *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int update_data_addresses(Label *head, int IC)
{
    Label *temp = head->next;
    while (temp != NULL)
    {
        if (temp->type == DATA)
        {
            temp->address += IC;
        }
        temp = temp->next;
    }
    return 0;
}