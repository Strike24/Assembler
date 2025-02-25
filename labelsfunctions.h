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

typedef struct
{
    char *name;
    int address;
    LabelType type;
    Label *next;
} Label;

Label *init_label_table();
Label *find_label(Label *head, int address);

int add_label(Label *head, char *name, int address, LabelType type);
void free_label_table(Label *head);
