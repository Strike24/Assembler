#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "binaryfunctions.h"
#include "defs.h"
#include "filefunctions.h"
#include "labelsfunctions.h"
#include "operations.h"
#include "macrofunctions.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Main first pass function. reads file after preasm and sends each line for parsing
First pass handles the encoding of source lines into binary lines and the saving of them in linked lists.
Also, first pass handles errors in the program and notifies about them*/
int first_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, MacroNode *macro_list, int *IC, int *DC);

/*Parses line and updates IC, DC, label list and label indicator.
Addes to data and code images
Returns 1 if error was found and 0 if not*/
int parse_line(char *line, int *IC, int *DC, int line_number, int *is_label, int *memory_exceeded, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, MacroNode *macro_list);

/*Gives line data and vaildation.
Returns operation type, line validation indicator and label indicator*/
LabelType validate_line(char *line, ErrorObject *error, int *is_label, int line_number, MacroNode *macro_list);
/*Trims a string from spaces and new lines*/
void trim(char *str);
#endif