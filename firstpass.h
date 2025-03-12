#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "binaryfunctions.h"
#include "defs.h"
#include "filefunctions.h"
#include "labelsfunctions.h"
#include "operations.h"
#include "macrofunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Main first pass function. reads file after preasm and sends each line for parsing*/
int first_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list);

/*Parses line and updates IC, DC, label list and label indicator.
Addes to data and code images*/
int parse_line(char *line, int *IC, int *DC, int line_number, int *is_label, BinaryNode *code_image, BinaryNode *data_image, Label *label_list);

/*Gives line data and vaildation.
Returns operation type, line validation indicator and label indicator*/
LabelType validate_line(char *line, int *is_valid, int *is_label);
void trim(char *str);
#endif