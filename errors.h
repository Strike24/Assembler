#ifndef ERRORS_H
#define ERRORS_H

#define NUMBER_OF_ERRORS 23
#define NUMBER_OF_WARNINGS 1
#define SUCCESS 0
#define FAILURE 1
#define NO_LINE -1

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ERROR_MEMORY_ALLOCATION_FAILED = 2,
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
    ERROR_LABEL_IS_MACRO_NAME,
    ERROR_INVALID_OPERATION_TYPE,
    ERROR_INVALID_NUMBER,
    ERROR_INVALID_STRING,
    ERROR_INVALID_OPERAND_TYPE,
    ERROR_INVALID_AMOUNT_OF_OPERANDS,
    ERROR_OPERAND_NOT_ALLOWED,
    ERROR_MACRO_NAME_TOO_LONG,
    ERROR_MACRO_NOT_ALPHANUMERIC,
    ERROR_MACRO_NOT_DEFINED,
    ERROR_MACRO_RESERVED_WORD,
    ERROR_MACRO_INVALID_START,
    ERROR_MACRO_EMPTY_NAME
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

typedef struct
{
    ErrorCode code;
    char extra_word[MAX_LINE];
    int line_number;
} ErrorObject;

void handleWarning(WarningCode code);
void handle_error(ErrorObject *error);
void handle_line_warning(WarningCode code, int line_number, char *extra_word);
int fill_error_object(ErrorCode code, int line_number, char *extra_word, ErrorObject *error);
void handle_system_error(ErrorCode code);
#endif