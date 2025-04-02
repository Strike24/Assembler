#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include "errors.h"
#include "binaryfunctions.h"
#include "labelsfunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*A function to handle file opening with different modes and extensions.
Handles errors and returns NULL if file could not be opened.
Returns pointer to the file struct*/
FILE *open_file(char *filename, char *mode, char *ext);

/*A function to remove a file with a given extension
Handles errors. Returns void.*/
void remove_file(char *filename, char *ext);

/*Builds the final output files. object file, entries file, and externs file.*/
int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, ExternLabel *extern_list, int ICF, int DCF);
/*Helper function for building output files.
Takes binary code and prints it to a file with addresses*/
int print_line_ob(FILE *ob_file, BinaryLine *current_node, int starting_index);

/*Helper functions for file creation*/

FILE *create_ob_file(BinaryNode *code_image, BinaryNode *data_image, char *filename);
FILE *create_ent_file(Label *label_list, char *filename, int *is_ent_file);
FILE *create_ext_file(ExternLabel *extern_list, char *filename, int *is_ext_file);
#endif
