#ifndef BINARYFUNCTIONS_H
#define BINARYFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "operations.h"
#include "firstpass.h"
#define OPCODE_OFFSET 18
#define FUNCT_OFFSET 3
#define DEST_REG_OFFSET 8
#define SRC_REG_OFFSET 13
#define ADD_MTD_SRC_OFFSET 16
#define ADD_MTD_DEST_OFFSET 11
#define A_OFFSET 2
#define R_OFFSET 1
#define E_OFFSET 0

typedef struct BinaryLine
{
    int address;
    unsigned int code;
} BinaryLine;

BinaryLine code_binary(int *IC, char *line);
BinaryLine data_binary(int *DC, char *line);
unsigned int get_code_for_operation(Operation *pOperation);
unsigned int get_code_for_register(char *register_name, int is_source);

#endif