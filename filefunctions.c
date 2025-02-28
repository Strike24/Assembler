#include "filefunctions.h"

void add_extension(char *filename, char *extension)
{
    if (check_extension(filename, ".as") == 0)
    {
        strcat(filename, extension);
    }
}

int check_extension(char *filename, char *extension)
{
    char *dot = strrchr(filename, '.');
    if (dot == NULL)
    {
        return 0;
    }
    return strcmp(dot, extension) == 0;
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

FILE *open_file(char *filename, char *mode, char *ext)
{
    char *pFileName = (char *)malloc(strlen(filename) + strlen(ext) + 1);
    FILE *file;
    memset(pFileName, 0, strlen(filename) + strlen(ext) + 1);

    if (pFileName == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    strcpy(pFileName, filename);
    strcat(pFileName, ext);

    file = fopen(pFileName, mode);

    free(pFileName); /*no longer needed*/

    if (file == NULL)
    {
        printf("Error: File not found\n");
        return NULL;
    }

    return file;
}