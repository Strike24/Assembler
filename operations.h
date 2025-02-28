#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
/*Types of addressing methods*/
typedef enum
{
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
    OperandType allowed_source[3]; /*Allowed addressing methods for source*/
    OperandType allowed_target[3]; /*Allowed addressing methods for target*/

} Operation;

/* checks if a string is one of the operation names */
int is_operation_name(char *name);
/* check if a string is one of the register names */
int is_register_name(char *name);

#endif