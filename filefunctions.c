#include "filefunctions.h"

void add_extension(char *filename, char *extension)
{
    if (check_extension(filename) == 0)
    {
        strcat(filename, extension);
    }
}

int check_extension(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL)
    {
        return 0;
    }
    return 1;
}

void replace_extension(char *filename, char *extension)
{
    char *dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }
    strcat(filename, extension);
}