#include "errors.h"

int fill_error_object(ErrorCode code, int line_number, char *extra_word, ErrorObject *error)
{
    if (!error)
    {
        return FAILURE;
    }

    error->code = code;
    error->line_number = line_number;
    if (extra_word != NULL && strlen(extra_word) < MAX_LINE)
        strcpy(error->extra_word, extra_word);
    return SUCCESS;
}

void handle_system_error(ErrorCode code)
{
    ErrorObject error = {0};
    error.code = code;
    error.line_number = NO_LINE;
    handle_error(&error);
    return;
}

void handle_error(ErrorObject *error)
{
    Error errors[SIZEOF_ERRORS_ARRAY] = {
        {ERROR_MEMORY_ALLOCATION_FAILED, "Memory allocation failed, exiting the program.\n", 0},
        {ERROR_FILE_NOT_FOUND, "File with filename \"%s\" not found in the file system.\n", 1},
        {ERROR_FILE_OPEN_FAILED, "Could not create new file.\n", 0},
        {ERROR_FILE_CLOSE_FAILED, "Could not close the file.\n", 0},
        {ERROR_LINE_TOO_LONG, "Line exceeds maximum amount of characters %s.\n", 1},
        {ERROR_LABEL_ALREADY_EXISTS, "Label named \"%s\" already decleared.\n", 1},
        {ERROR_LABEL_NOT_DEFINED, "Label named \"%s\" is not defined.\n", 1},
        {ERROR_LABEL_NAME_TOO_LONG, "Label name \"%s\" exceeds maximum amount of 30 characters.\n", 1},
        {ERROR_LABEL_INVALID_START, "Label name \"%s\" must start with an alphabetic character.\n", 1},
        {ERROR_LABEL_EMPTY_NAME, "Label name cannot be empty.\n", 0},
        {ERROR_LABEL_NOT_ALPHANUMERIC, "Label name \"%s\" must contain only alphabetic characters and digits.\n", 1},
        {ERROR_LABEL_RESERVED_WORD, "Label name \"%s\" is a reserved word.\n", 1},
        {ERROR_LABEL_IS_MACRO_NAME, "Label name \"%s\" is a macro name.\n", 1},
        {ERROR_LABEL_EMPTY_LINE, "Label can't be decleared on an empty line\n", 0},
        {ERROR_LABEL_IS_EXTERNAL, "Label \"%s\" is an external label and cannot be used as an entry.\n", 1},
        {ERROR_INVALID_OPERATION_TYPE, "Word \"%s\" is not a defined operation / instruction type.\n", 1},
        {ERROR_INVALID_NUMBER, "One or more of the .data paramters are not valid numbers.\n", 0},
        {ERROR_INVALID_STRING, "%s is not a valid string.\n", 1},
        {ERROR_INVALID_OPERAND_TYPE, "\"%s\" is not a valid operand type.\n", 1},
        {ERROR_INVALID_AMOUNT_OF_OPERANDS, "Invalid amount of operands for operation \"%s\".\n", 1},
        {ERROR_OPERAND_NOT_ALLOWED, "Invalid operand type for operation \"%s\".\n", 1},
        {ERROR_MACRO_NAME_TOO_LONG, "Macro name \"%s\" exceeds maximum amount of 30 characters.\n", 1},
        {ERROR_MACRO_NOT_ALPHANUMERIC, "Macro name \"%s\" must contain only alphabetic characters, digits, and underscores.\n", 1},
        {ERROR_MACRO_NOT_DEFINED, "Macro \"%s\" is not defined.\n", 1},
        {ERROR_MACRO_RESERVED_WORD, "Macro name \"%s\" is a reserved word.\n", 1},
        {ERROR_MACRO_INVALID_START, "Macro name \"%s\" must start with an alphabetic character or underscore.\n", 1},
        {ERROR_MACRO_EMPTY_NAME, "Macro name cannot be empty.\n", 0},
        {ERROR_MACRO_ALREADY_EXISTS, "Macro named \"%s\" already decleared.\n", 1},
        {ERROR_MEMORY_EXCEEDED, "* Memory address exceeded 2^21, only validating input from now on. *\n", 0},
    };
    int i;

    if (!error || error->code == SUCCESS)
    {
        return;
    }
    if (error->code == ERROR_MEMORY_ALLOCATION_FAILED)
    {
        printf("CRITICAL ERROR (memory or other)\n");
        /*TODO: FREE ALL MEMORY HERE*/
        exit(1);
        return;
    }
    /*Loop over errors array until error code is found*/
    for (i = 0; i < SIZEOF_ERRORS_ARRAY; i++)
    {
        if (errors[i].code == error->code)
        {
            if (error->line_number != NO_LINE)
            {
                printf("ERROR in line %d:\n", error->line_number);
                printf("\t");
                if (errors[i].is_extra_word && error->extra_word != NULL)
                {
                    printf(errors[i].message, error->extra_word);
                }
                else
                {
                    printf("%s", errors[i].message);
                }
                printf("\n");
                return;
            }
            else
            {
                printf("ERROR:\n");
                printf("\t");
                if (errors[i].is_extra_word && error->extra_word != NULL)
                {
                    printf(errors[i].message, error->extra_word);
                }
                else
                {
                    printf("%s", errors[i].message);
                }
                printf("\n");
                return;
            }
        }
    }
}
