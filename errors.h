#ifndef ERRORS_H
#define ERRORS_H

#define NUMBER_OF_ERRORS 7
#define NUMBER_OF_WARNINGS 1
#include <stdio.h>
typedef enum
{
    ERROR_MEMORY_ALLOCATION_FAILED,
    ERROR_FILE_NOT_FOUND,
    ERROR_FILE_OPEN_FAILED,
    ERROR_FILE_CLOSE_FAILED,
    ERROR_LINE_TOO_LONG,
    ERROR_LABEL_ALREADY_EXISTS,
    ERROR_LABEL_NOT_DEFINED,
    ERROR_INVALID_OPERATION_TYPE
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