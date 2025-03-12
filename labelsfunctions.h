#ifndef LABELSFUNCTIONS_H
#define LABELSFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

typedef enum
{
    CODE,
    DATA,
    EXTERNAL,
    ENTRY
} LabelType;

typedef struct Label
{
    char *name;
    int address;
    LabelType type;
    struct Label *next;
    struct Label *prev;
} Label;

typedef struct ExternLabel
{
    Label *label;
    int usage_address;
    struct ExternLabel *next;
    struct ExternLabel *prev;
} ExternLabel;
Label *init_label_table();
Label *find_label(Label *head, char *name);
ExternLabel *init_extern_label_table();
void free_extern_label_table(ExternLabel *head);
int add_extern_label(ExternLabel *head, Label *label, int address);
int add_label(Label *head, char *name, int address, LabelType type);
void free_label_table(Label *head);
int update_data_addresses(Label *head, int IC);
int add_extern_label_to_list(char *line, Label *head);

#endif