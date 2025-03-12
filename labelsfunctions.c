#include "labelsfunctions.h"

Label *init_label_table()
{
    Label *head = (Label *)malloc(sizeof(Label));
    if (!head)
    {
        return NULL;
    }
    head->name = NULL;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

Label *find_label(Label *head, char *name)
{
    Label *temp = head;
    while (temp != NULL)
    {
        if ((temp->name != NULL) && (strcmp(temp->name, name) == 0))
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int add_label(Label *head, char *name, int address, LabelType type)
{
    Label *newLabel;
    if (find_label(head, name) != NULL)
    {
        return ERROR;
    }

    newLabel = (Label *)malloc(sizeof(Label));
    if (!newLabel)
    {
        return ERROR;
    }
    newLabel->name = (char *)malloc(strlen(name) + 1);
    if (!newLabel->name)
    {
        free(newLabel);
        return ERROR;
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

int add_extern_label(char *line, Label *head)
{
    char *word;
    word = strtok(line, " \t\n");
    word = strtok(NULL, " \t\n");
    add_label(head, word, 0, EXTERNAL);
    return 0;
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