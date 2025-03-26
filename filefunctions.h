#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include "errors.h"
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

#endif
