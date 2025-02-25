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

/* Checks if a macro name is valid, returns 1 if valid else 0 */
int is_valid_macro_name(char *name);
void print_macro_table(MacroNode *head);

int add_macro2(MacroNode *head, char *name, char *content, int len);
void get_macro_content(char *macro_content, int len);
