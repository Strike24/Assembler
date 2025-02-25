#include "macrofunctions.h"

MacroNode *init_macro_table()
{
    MacroNode *head = (MacroNode *)malloc(sizeof(MacroNode));
    head->macro = NULL;
    head->next = NULL;
    return head;
}

int add_content_to_macro(MacroNode *macro_node, char *content, int len)
{
    macro_node->macro->content = (char *)malloc(len + 1);
    /*
    if (!newMacro->content)
    {
        free(newMacro->name);
        free(newMacro);
        return ERROR;
    }
        */

    strcpy(macro_node->macro->content, content);
    return 0;
}

int add_macro(MacroNode *head, char *name)
{
    Macro *newMacro = (Macro *)malloc(sizeof(Macro));
    MacroNode *newNode = (MacroNode *)malloc(sizeof(MacroNode));
    if (!newMacro || !newNode)
        return ERROR;
    newMacro->name = (char *)malloc(strlen(name) + 1);
    strcpy(newMacro->name, name);
    newMacro->content = (char *)malloc(MAX_LINE * 100 * sizeof(char));
    if (!newMacro->content)
    {
        free(newMacro->name);
        free(newMacro);
        return ERROR;
    }
    newMacro->content[0] = '\0';

    newNode->macro = newMacro;
    newNode->next = head->next;
    head->next = newNode;
    return 0;
}

void free_macro_table(MacroNode *head)
{
    MacroNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        if (temp->macro)
        {
            free(temp->macro->name);
            free(temp->macro->content);
            free(temp->macro);
        }
        free(temp);
    }
}

Macro *find_macro(MacroNode *head, char *name)
{
    MacroNode *temp = head->next;
    while (temp != NULL)
    {
        if (strcmp(temp->macro->name, name) == 0)
        {
            return temp->macro;
        }
        temp = temp->next;
    }
    return NULL;
}

Macro *get_current_macro(MacroNode *head)
{
    MacroNode *temp;
    if (head == NULL || head->next == NULL)
        return NULL;
    temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp->macro;
}

int is_valid_macro_name(char *name)
{
    if (name == NULL)
        return 0;

    if (*name == '\0')
        return 0;

    return 1;
}

/*DEBUG REMOVE BEFORE PUBLISHING*/
void print_macro_table(MacroNode *head)
{
    MacroNode *temp = head->next;
    while (temp != NULL)
    {
        printf("\n\n--------------\nMacro name: %s\n", temp->macro->name);
        printf("Macro content: \n%s", temp->macro->content);
        temp = temp->next;
    }
}