#include "filefunctions.h"

FILE *open_file(char *filename, char *mode, char *ext)
{
    ErrorObject error = {0};
    /*Allocates memory for a new file name that includes the extension*/
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
    FILE *file = NULL;

    if (ext_filename == NULL)
    {
        /*If memory allocation failed, handle the error and return NULL*/
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }

    /*Initializes all chars to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);

    /*Copies the original file name to the new file name*/
    strcpy(ext_filename, filename);
    /*Adds the extension to the new file name*/
    strcat(ext_filename, ext);

    /*Opens the file with the new file name and the mode*/
    file = fopen(ext_filename, mode);

    free(ext_filename); /*ext_file no longer needed, free allocation*/

    if (file == NULL)
    {
        if (strcmp(mode, "r") == 0)
        {
            /*Failed to open file for reading, handle error and return null*/
            fill_error_object(ERROR_FILE_NOT_FOUND, 0, filename, &error);
            handle_error(&error);
        }
        else
        {
            /*Failed to open file for writing, handle error and return null*/
            fill_error_object(ERROR_FILE_OPEN_FAILED, 0, filename, &error);
            handle_error(&error);
        }
        return NULL;
    }

    return file;
}

void remove_file(char *filename, char *ext)
{
    /*Allocates memory for a new file name that includes the extension*/
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        /*If memory allocation failed, handle the error and return*/
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return;
    }

    /*Initializes all chars to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);
    /*Copies the original file name to the new file name*/
    strcpy(ext_filename, filename);
    /*Adds the extension to the new file name*/
    strcat(ext_filename, ext);

    /*Removes the file with the new file name*/
    remove(ext_filename);

    free(ext_filename);
}