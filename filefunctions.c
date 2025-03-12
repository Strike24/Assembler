#include "filefunctions.h"

FILE *open_file(char *filename, char *mode, char *ext)
{
    /*Allocates memory for a new file name that includes the extension*/
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
    FILE *file;
    /*Initializes all to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    strcpy(ext_filename, filename);
    strcat(ext_filename, ext);

    file = fopen(ext_filename, mode);

    free(ext_filename); /*ext_file no longer needed, free allocation*/

    if (file == NULL)
    {
        printf("Error: File not found\n");
        return NULL;
    }

    return file;
}