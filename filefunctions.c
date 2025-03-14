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
        handleError(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    strcpy(ext_filename, filename);
    strcat(ext_filename, ext);

    file = fopen(ext_filename, mode);

    free(ext_filename); /*ext_file no longer needed, free allocation*/

    if (file == NULL)
    {
        if (strcmp(mode, "r") == 0)
        {
            handleError(ERROR_FILE_NOT_FOUND);
        }
        else
        {
            handleError(ERROR_FILE_OPEN_FAILED);
        }
        return NULL;
    }

    return file;
}

void remove_file(char *filename, char *ext)
{
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        handleError(ERROR_MEMORY_ALLOCATION_FAILED);
        return;
    }
    strcpy(ext_filename, filename);
    strcat(ext_filename, ext);

    remove(ext_filename);

    free(ext_filename);
}