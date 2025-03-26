#include "macrofunctions.h"

MacroNode *init_macro_table()
{
    MacroNode *head = (MacroNode *)malloc(sizeof(MacroNode));
    if (!head)
    {
        free(head);
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    head->macro = NULL;
    head->next = NULL;
    return head;
}

ErrorCode add_content_to_macro(MacroNode *macro_node, char *content, int len)
{
    macro_node->macro->content = (char *)malloc(len + 1);
    if (!macro_node->macro->content)
    {
        free(macro_node->macro->name);
        free(macro_node->macro);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    /*
    if (!newMacro->content)
    {
        free(newMacro->name);
        free(newMacro);
        return ERROR;
    }
        */

    strcpy(macro_node->macro->content, content);
    return SUCCESS;
}

ErrorCode add_macro(MacroNode *head, char *name)
{
    Macro *newMacro = (Macro *)malloc(sizeof(Macro));
    MacroNode *newNode = (MacroNode *)malloc(sizeof(MacroNode));
    if (!newMacro)
        return ERROR_MEMORY_ALLOCATION_FAILED;
    if (!newNode)
    {
        free(newMacro);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    newMacro->name = (char *)malloc(strlen(name) + 1);
    if (!newMacro->name)
    {
        free(newMacro);
        free(newNode);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    strcpy(newMacro->name, name);
    newMacro->content = (char *)malloc(MAX_LINE * MAX_MACRO_LINES * sizeof(char));
    if (!newMacro->content)
    {
        free(newMacro->name);
        free(newMacro);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }
    newMacro->content[0] = '\0';

    newNode->macro = newMacro;
    newNode->next = head->next;
    head->next = newNode;
    return SUCCESS;
}

void free_macro_table(MacroNode *head)
{
    MacroNode *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        if (temp->macro)
        {
            if (temp->macro->name != NULL)
                free(temp->macro->name);
            if (temp->macro->content != NULL)
                free(temp->macro->content);
            free(temp->macro);
        }
        free(temp);
    }
}

Macro *find_macro(MacroNode *head, char *name)
{
    MacroNode *temp;
    if (head == NULL)
        return NULL;
    temp = head->next;
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

ErrorCode validate_macro_name(char *name, MacroNode *head)
{
    if (name == NULL)
        return ERROR_NULL_PARAM;
    if (strlen(name) == 0)
        return ERROR_MACRO_EMPTY_NAME;
    if (strlen(name) > MAX_MACRO_NAME)
        return ERROR_MACRO_NAME_TOO_LONG;
    if (!isalpha(name[0]))
        return ERROR_MACRO_INVALID_START;

    if (is_reserved_word(name))
        return ERROR_MACRO_RESERVED_WORD;

    if (find_macro(head, name) != NULL)
        return ERROR_MACRO_ALREADY_EXISTS;

    while (*name)
    {
        if (!isalnum(*name) && *name != '_')
            return ERROR_MACRO_NOT_ALPHANUMERIC;
        name++;
    }
    return SUCCESS;
}

int is_macro(char *name, MacroNode *head)
{
    if (find_macro(head, name) != NULL)
        return TRUE;
    return FALSE;
}