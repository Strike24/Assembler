#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "defs.h"
#include "filefunctions.h"
#include "labelsfunctions.h"
#include "operations.h"
#include "macrofunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Main first pass function. reads file after preasm and sends each line for parsing*/
int first_pass(char *filename);

/*Parses line and updates IC, DC, label list and label indicator*/
int parse_line(char *line, int *IC, int *DC, int *is_label, Label *label_list);

/*Gives line data and vaildation.
Returns operation type, line validation indicator and label indicator*/
LabelType validate_line(char *line, int *is_valid, int *is_label);

#endif