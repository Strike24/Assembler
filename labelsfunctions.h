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
} Label;

Label *init_label_table();
Label *find_label(Label *head, char *name);

int add_label(Label *head, char *name, int address, LabelType type);
void free_label_table(Label *head);
int add_extern_label(char *line, Label *head);
int update_data_addresses(Label *head, int IC);
#endif