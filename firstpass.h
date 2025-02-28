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
#include <ctype.h>

/*First pass functions*/
int first_pass(char *filename);

/*Parses line and updates IC, DC, label list and label indicator*/
int parse_line(char *line, int *IC, int *DC, int *is_label, Label *label_list);

/*Gives line data and vaildation.
Returns operation type, line validation indicator and label indicator.*/
LabelType validate_line(char *line, int *is_valid, int *is_label);

/*Detect operation based on the name*/
int is_data_operation(char *word);
int is_extern_operation(char *word);
int is_entry_operation(char *word);
int is_code_operation(char *word);
int is_label_dec(char *word);
int is_reserved_word(char *word);

/*Validate line data based on operation*/
int validate_data(char *word);
int validate_extern(char *word);
int validate_entry(char *word);
int validate_code(char *word);

#endif