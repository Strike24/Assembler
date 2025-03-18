#ifndef ERRORS_H
#define ERRORS_H

#define NUMBER_OF_ERRORS 18
#define NUMBER_OF_WARNINGS 1
#include <stdio.h>
typedef enum
{
    ERROR_MEMORY_ALLOCATION_FAILED = 1,
    ERROR_FILE_NOT_FOUND,
    ERROR_FILE_OPEN_FAILED,
    ERROR_FILE_CLOSE_FAILED,
    ERROR_LINE_TOO_LONG,
    ERROR_LABEL_ALREADY_EXISTS,
    ERROR_LABEL_NOT_DEFINED,
    ERROR_LABEL_NAME_TOO_LONG,
    ERROR_LABEL_INVALID_START,
    ERROR_LABEL_EMPTY_NAME,
    ERROR_LABEL_NOT_ALPHANUMERIC,
    ERROR_LABEL_RESERVED_WORD,
    ERROR_INVALID_OPERATION_TYPE,
    ERROR_INVALID_NUMBER,
    ERROR_INVALID_STRING,
    ERROR_INVALID_OPERAND_TYPE,
    ERROR_INVALID_AMOUNT_OF_OPERANDS,
    ERROR_OPERAND_NOT_ALLOWED
} ErrorCode;

typedef enum
{
    IGNORED_LABEL
} WarningCode;

typedef struct
{
    ErrorCode code;
    char *message;
    char is_extra_word;
} Error;

typedef struct
{
    WarningCode code;
    char *message;
    char is_extra_word;
} Warning;

void handleError(ErrorCode code);
void handleWarning(WarningCode code);
void handle_line_error(ErrorCode code, int line_number, char *extra_word);
void handle_line_warning(WarningCode code, int line_number, char *extra_word);

#endif