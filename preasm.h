#ifndef PREASM_H
#define PREASM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filefunctions.h"
#include "macrofunctions.h"
#include "defs.h"
#include "firstpass.h"

/*The Pre Assembler handles the process of macro expansion. Main function to open the as file and create an am file*/
int pre_assembler(char *filename, MacroNode *head);

/*Loops over every line in the .as file and expands created macros. Outputs .am file with macros expanded
the .am file will be used in the next passes*/
int macro_expansion(FILE *input_file, FILE *output_file, MacroNode *head);

#endif