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

/*Main first pass function. reads file after preasm and sends each line for parsing*/
int first_pass(char *filename);

/*Parses line and updates IC, DC, label list and label indicator*/
int parse_line(char *line, int *IC, int *DC, int *is_label, Label *label_list);

/*Gives line data and vaildation.
Returns operation type, line validation indicator and label indicator*/
LabelType validate_line(char *line, int *is_valid, int *is_label);

/*Operation Type Detection*/

/*Detect data operation based on the name*/
int is_data_operation(char *word);
/*Detect extern operation based on the name*/
int is_extern_operation(char *word);
/*Detect entry operation based on the name*/
int is_entry_operation(char *word);
/*Detect code operation based on the name*/
int is_code_operation(char *word);
/*Detect if word is a label declaration*/
int is_label_dec(char *word);
/*Detect if word is a reserved word*/
int is_reserved_word(char *word);

/*Validate line operands and format based on operation*/

int validate_data(char *word);
int validate_extern(char *word);
int validate_entry(char *word);
/*Validates format of operands and allowed addressing methods based on operation name.*/
int validate_code(char *word);

#endif