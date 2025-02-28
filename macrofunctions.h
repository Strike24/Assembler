#ifndef MACROFUNCTIONS_H
#define MACROFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

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

MacroNode *init_macro_table();

int add_macro(MacroNode *head, char *name);

void free_macro_table(MacroNode *head);

Macro *find_macro(MacroNode *head, char *name);

Macro *get_current_macro(MacroNode *head);

void print_macro_table(MacroNode *head);

int is_macro(char *word);

#endif