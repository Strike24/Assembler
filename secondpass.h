#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "binaryfunctions.h"
#include "defs.h"
#include "filefunctions.h"
#include "labelsfunctions.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Main second pass function.
The second pass handles filling the label addresses, the .entry and .extern labels, and the building of the output files.*/
int second_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, int ICF, int DCF, int is_error);

/*A function to fill missing addresses of labels in the command operands.
Based on a given line, finds if there is a label to fill, searches for the address, and fills it in the binary code.*/
int fill_missing_label_info(BinaryNode *code_image, Label *label_list, char *line, int line_number, ExternLabel *extern_list, ErrorObject *error);

/*Builds the final output files. object file, entries file, and externs file.*/
int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, ExternLabel *extern_list, int ICF, int DCF);
/*Helper function for building output files.
Takes binary code and prints it to a file with addresses*/
int print_line_ob(FILE *ob_file, BinaryLine *current_node, int starting_index);
/*Aligns all of the binary lines to fit 24 bits.\
Applys a mask to insure every bit after the 23'rd bit is off (Max )*/
int align_memory_to_bits(BinaryNode *code_image, BinaryNode *data_image);
#endif