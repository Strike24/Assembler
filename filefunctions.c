#include "filefunctions.h"

FILE *open_file(char *filename, char *mode, char *ext)
{
    /*Allocates memory for a new file name that includes the extension*/
    ErrorObject error = {0};
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
    FILE *file = NULL;

    if (ext_filename == NULL)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }

    /*Initializes all to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);
    strcpy(ext_filename, filename);
    strcat(ext_filename, ext);

    file = fopen(ext_filename, mode);

    free(ext_filename); /*ext_file no longer needed, free allocation*/

    if (file == NULL)
    {
        if (strcmp(mode, "r") == 0)
        {
            fill_error_object(ERROR_FILE_NOT_FOUND, 0, filename, &error);
            handle_error(&error);
        }
        else
        {
            fill_error_object(ERROR_FILE_OPEN_FAILED, 0, filename, &error);
            handle_error(&error);
        }
        return NULL;
    }

    return file;
}

void remove_file(char *filename, char *ext)
{
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return;
    }

    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);
    strcpy(ext_filename, filename);
    strcat(ext_filename, ext);

    remove(ext_filename);

    free(ext_filename);
}