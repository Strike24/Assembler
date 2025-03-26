#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "preasm.h"
#include "firstpass.h"
#include "secondpass.h"

/*Free all linked lists in one place*/
void free_all(MacroNode *macro_list, BinaryNode *code_image, BinaryNode *data_image, Label *label_list);

#endif