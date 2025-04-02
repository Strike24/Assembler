#include "macrofunctions.h"

MacroNode *init_macro_table()
{
    MacroNode *head = (MacroNode *)malloc(sizeof(MacroNode)); /*Allocate size of macro list head*/
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

ErrorCode add_macro(MacroNode *head, char *name)
{
    Macro *newMacro = (Macro *)malloc(sizeof(Macro));            /*Allocate size of macro*/
    MacroNode *newNode = (MacroNode *)malloc(sizeof(MacroNode)); /*Allocate size of macro node*/

    if (!head || !name)
        return ERROR_NULL_PARAM;

    if (!newMacro)
        return ERROR_MEMORY_ALLOCATION_FAILED;
    if (!newNode)
    {
        free(newMacro);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }

    newMacro->name = (char *)malloc(strlen(name) + 1); /*Allocate size of macro name*/
    if (!newMacro->name)
    {
        free(newMacro);
        free(newNode);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }

    strcpy(newMacro->name, name);                                                  /*Copy name to macro name*/
    newMacro->content = (char *)malloc(MAX_LINE * MAX_MACRO_LINES * sizeof(char)); /*Allocate size of macro content to be 80 lines*/
    if (!newMacro->content)
    {
        free(newMacro->name);
        free(newMacro);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }

    newMacro->content[0] = '\0'; /*Initialize content to empty string*/

    newNode->macro = newMacro;
    newNode->next = head->next;
    head->next = newNode;
    return SUCCESS;
}

void free_macro_table(MacroNode *head)
{
    MacroNode *temp = NULL;

    if (!head)
    {
        return;
    }

    /*Loops over macro list and free every macro*/
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

    /*Loop over macro list and search for macro by name*/
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

    /*Get last macro in the list*/
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

    /*Validate macro name length*/
    if (strlen(name) == 0)
        return ERROR_MACRO_EMPTY_NAME;
    if (strlen(name) > MAX_MACRO_NAME)
        return ERROR_MACRO_NAME_TOO_LONG;

    /*Check if first letter is alphabetic*/
    if (!isalpha(name[0]) && name[0] != '_')
        return ERROR_MACRO_INVALID_START;

    if (is_reserved_word(name))
        return ERROR_MACRO_RESERVED_WORD;

    /*Check if macro already created*/
    if (head && find_macro(head, name) != NULL)
        return ERROR_MACRO_ALREADY_EXISTS;

    /*Check if all chars are alphanumeric or _ */
    while (name && (name[0] != '\0'))
    {
        if (!isalnum(name[0]) && (name[0] != '_'))
            return ERROR_MACRO_NOT_ALPHANUMERIC;
        name++;
    }
    return SUCCESS;
}

int is_macro(char *name, MacroNode *head)
{
    if (!name || !head)
        return FALSE;
    if (find_macro(head, name) != NULL)
        return TRUE;
    return FALSE;
}