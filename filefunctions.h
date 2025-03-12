#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*A function that handles file opening with different modes and extensions.
Returns pointer to the create file*/
/*TODO: MOVE TO DIFFERENT FILE*/
FILE *open_file(char *filename, char *mode, char *ext);

#endif
