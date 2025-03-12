#ifndef LABELSFUNCTIONS_H
#define LABELSFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

/*Enum that represents the types of labels*/
typedef enum
{
    CODE,
    DATA,
    EXTERNAL,
    ENTRY
} LabelType;

/*Represents a label, holds a name, address, type
Also acts as a node of the linked list*/
typedef struct Label
{
    char *name;
    int address;
    LabelType type;
    struct Label *next;
    struct Label *prev;
} Label;

/*Represents an extern label, acts as a node in the list
The list holds all extern labels and their usages in the program*/
typedef struct ExternLabel
{
    Label *label;
    int usage_address;
    struct ExternLabel *next;
    struct ExternLabel *prev;
} ExternLabel;

/*Initialize linked lists, returns pointer to head*/

Label *init_label_table();
ExternLabel *init_extern_label_table();

/*Finds a label by name, returns pointer to it if found*/
Label *find_label(Label *head, char *name);
/*Adds an extern label to the list*/
int add_extern_label(ExternLabel *head, Label *label, int address);
/*Adds a label to the list*/
int add_label(Label *head, char *name, int address, LabelType type);
/*Frees the normal label list*/
void free_label_table(Label *head);
/*Frees the extern label list*/
void free_extern_label_table(ExternLabel *head);
/*Updates the addresses of all data labels by a value*/
int update_data_addresses(Label *head, int IC);
/*Adds an extern label to the normal list*/
int add_extern_label_to_list(char *line, Label *head);

#endif