#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "binaryfunctions.h"
#include "defs.h"
#include "filefunctions.h"
#include "labelsfunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int second_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list);

/*A function to fill missing addresses of labels*/
int fill_missing_label_info(BinaryNode *code_image, Label *label_list, char *line, int line_number);

int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, int ICF, int DCF);
int print_line_ob(FILE *ob_file, BinaryLine *current_node);
#endif