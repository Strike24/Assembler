#ifndef OPERATIONS_H
#define OPERATIONS_H

#define SOURCE 0
#define TARGET 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "errors.h"
#include <ctype.h>

/*Types of addressing methods*/
typedef enum
{
    NONE = -1,
    IMMEDIATE,
    DIRECT,
    RELATIVE,
    REGISTER
} OperandType;

/*Defines a code operation*/
typedef struct
{
    char *name;
    int opcode;
    int funct;
    OperandType allowed_source[MAX_OPERAND_TYPES]; /*Allowed addressing methods for source*/
    OperandType allowed_target[MAX_OPERAND_TYPES]; /*Allowed addressing methods for target*/

} Operation;

/* checks if a string is one of the operation names */
int is_operation_name(char *name);
/* check if a string is one of the register names */
int is_register_name(char *name);
int get_register_index(char *register_name);
Operation *get_operation(char *name);
OperandType *get_allowed_addressing_methods(char *name, int source_or_target);

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
int is_label_dec(char *word, int line_number);
/*Detect if word is a reserved word*/
int is_reserved_word(char *word);

/*Validate line operands and format based on operation*/

int validate_data(char *word, int line_number);
int validate_extern(char *word, int line_number);
int validate_entry(char *word, int line_number);
int validate_string(char *word, int line_number);
/*Validates format of operands and allowed addressing methods based on operation name.*/
int validate_code(char *operation, char *oprands, int line_number);
int is_integer(char *str, int len);
int is_legal_label_name(char *word, int line_number);

/*Addressing Method Validation*/

int is_immediate(char *word);
int is_direct(char *word);
int is_relative(char *word);
int is_register(char *word);
int get_operand_type(char *word);
#endif