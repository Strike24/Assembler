#ifndef BINARYFUNCTIONS_H
#define BINARYFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "operations.h"

/*Defines a "binary line" that holds an address, an array of binary code lines (and size of them), and the source line number.
Every BinaryLine struct represents a single or multiple lines from the source file.*/
typedef struct BinaryLine
{
    int address;
    unsigned int *code;
    int num_of_lines;
    int sourcecode_line_number;
} BinaryLine;

/*A node for the dobubly linked list. contains a BinaryLine*/
typedef struct BinaryNode
{
    BinaryLine *line;
    struct BinaryNode *next;
    struct BinaryNode *prev;
} BinaryNode;

/*Function for converting source code of commands to binary code
Returns a BinaryLine with single or multiple binary lines that represent a command and its operands*/
BinaryLine code_binary(int *IC, char *line, int sourcecode_line_number);

/*Function for converting source code of data to binary code
Returns BinaryLine that represents the encoded data*/
BinaryLine data_binary(int *DC, char *line, int sourcecode_line_number);

/*Initialize linked list of BinaryNode(s), returns pointer to head*/
BinaryNode *init_binary_image();
int add_binary_line(BinaryLine binaryLine, BinaryNode *head);
void free_binary_image(BinaryNode *head);
/*Find a BinaryLine by its source code line number and returns a pointer to it*/
BinaryLine *find_by_line_number(BinaryNode *head, int line_number);

#include "firstpass.h"
/*Helper functions to encode command and data lines into binary*/

unsigned int get_code_for_operation(Operation *pOperation);             /*Returns the binary code for the operation*/
unsigned int get_code_for_register(char *register_name, int is_source); /*Returns the binary code based on a register operand*/
void add_line_for_immidiate(char *operand, BinaryLine *binaryLine);     /*Encodes an immidiate type operand*/
void add_empty_line(BinaryLine *binaryLine);                            /*Addes empty binary line to be filled in the second pass*/
int get_number_of_integers(char *line);                                 /*Get number of integers in a data operation*/
#endif