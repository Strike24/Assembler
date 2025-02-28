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
    return head;
}

Label *find_label(Label *head, char *name)
{
    Label *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
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