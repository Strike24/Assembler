#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filefunctions.h"
#include "macrofunctions.h"
#include "defs.h"
#include "firstpass.h"

int pre_assembler(char *filename);

int macro_expansion(FILE *input_file, FILE *output_file);
