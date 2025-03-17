#include "errors.h"

Error errors[] = {
    {ERROR_MEMORY_ALLOCATION_FAILED, "Memory allocation failed, exiting the program.\n", 0},
    {ERROR_FILE_NOT_FOUND, "File with filename \"%s\" not found in the file system.\n", 1},
    {ERROR_FILE_OPEN_FAILED, "Could not create new file.\n", 0},
    {ERROR_FILE_CLOSE_FAILED, "Could not close the file.\n", 0},
    {ERROR_LINE_TOO_LONG, "Line exceeds maximum amount of characters %s.\n", 1},
    {ERROR_LABEL_ALREADY_EXISTS, "Label named \"%s\" already decleared.\n", 1},
    {ERROR_LABEL_NOT_DEFINED, "Label named \"%s\" is not defined.\n", 1},
    {ERROR_LABEL_NAME_TOO_LONG, "Label name \"%s\" exceeds maximum amount of 31 characters.\n", 1},
    {ERROR_LABEL_INVALID_START, "Label name \"%s\" must start with an alphabetic character.\n", 1},
    {ERROR_LABEL_EMPTY_NAME, "Label name cannot be empty.\n", 0},
    {ERROR_LABEL_NOT_ALPHANUMERIC, "Label name \"%s\" must contain only alphabetic characters and digits.\n", 1},
    {ERROR_LABEL_RESERVED_WORD, "Label name \"%s\" is a reserved word.\n", 1},
    {ERROR_INVALID_OPERATION_TYPE, "Operation \"%s\" is not a defined operation type.\n", 1},
    {ERROR_INVALID_NUMBER, "\"%s\" is not a valid number.\n", 1},
    {ERROR_INVALID_STRING, "\"%s\" is not a valid string.\n", 1},
    {ERROR_INVALID_OPERAND_TYPE, "\"%s\" is not a valid operand type.\n", 1},
    {ERROR_INVALID_AMOUNT_OF_OPERANDS, "Invalid amount of operands for operation \"%s\".\n", 1},
    {ERROR_OPERAND_NOT_ALLOWED, "Invalid operand type for operation \"%s\".\n", 1}};

Warning warnings[] = {
    {IGNORED_LABEL, "Label cannot be decleared before .extern / .entry. \n\tLabel \"%s\" will be ignored by the assembler.\n", 1}};

void handleError(ErrorCode code)
{
    int i;
    /*Loop over errors array until code is found*/
    for (i = 0; i < NUMBER_OF_ERRORS; i++)
    {
        if (errors[i].code == code)
        {
            /*Print error message*/
            printf("ERROR: %s\n", errors[i].message);
            return;
        }
    }
}
void handleWarning(WarningCode code)
{
    /*Loop over warnings array until code is found*/
    int i;
    for (i = 0; i < NUMBER_OF_WARNINGS; i++)
    {
        if (warnings[i].code == code)
        {
            /*Print warning message*/
            printf("WARNING: %s\n", warnings[i].message);
            return;
        }
    }
}
void handle_line_error(ErrorCode code, int line_number, char *extra_word)
{
    int i;
    /*Loop over errors array until code is found*/
    for (i = 0; i < NUMBER_OF_ERRORS; i++)
    {
        if (errors[i].code == code)
        {
            /*Print error message*/
            printf("ERROR in line %d:\n", line_number);
            printf("\t");
            if (errors[i].is_extra_word && extra_word != NULL)
            {
                printf(errors[i].message, extra_word);
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
void handle_line_warning(WarningCode code, int line_number, char *extra_word)
{
    int i;
    /*Loop over warnings array until code is found*/
    for (i = 0; i < NUMBER_OF_WARNINGS; i++)
    {
        if (warnings[i].code == code)
        {
            /*Print warning message*/
            printf("WARNING in line %d:\n", line_number);
            printf("\t");
            if (warnings[i].is_extra_word)
            {
                if (extra_word != NULL)
                {
                    printf(warnings[i].message, extra_word);
                }
            }
            else
            {
                printf("%s", warnings[i].message);
            }
            printf("\n");
            return;
        }
    }
}