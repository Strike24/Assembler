#ifndef MACROFUNCTIONS_H
#define MACROFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "defs.h"
#include "errors.h"

typedef struct
{
    char *name;
    char *content;
} Macro;

typedef struct MacroNode
{
    Macro *macro;
    struct MacroNode *next;
} MacroNode;

/*Added after macro and macronode declaration because operations.h uses them*/
#include "operations.h"

/*Initializes the macro table*/
MacroNode *init_macro_table();
/*Adds a macro to the macro table*/
ErrorCode add_macro(MacroNode *head, char *name);

/*Free the macro table*/
void free_macro_table(MacroNode *head);

/*Get a macro pointer by its name*/
Macro *find_macro(MacroNode *head, char *name);

/*Get the last added macro in the list. (the one being worked on)*/
Macro *get_current_macro(MacroNode *head);

/*Validate macro name*/
ErrorCode validate_macro_name(char *name, MacroNode *head);

/*Returns if a name is a macro name*/
int is_macro(char *name, MacroNode *head);

#endif