#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_extension(char *filename, char *extension);

int check_extension(char *filename, char *extension);
void replace_extension(char *filename, char *extension);
FILE *open_file(char *filename, char *mode, char *ext);

#endif
