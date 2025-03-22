#ifndef ERRORS_H
#define ERRORS_H

#define SIZEOF_ERRORS_ARRAY 28
#define NUMBER_OF_WARNINGS 1
#define SUCCESS 0
#define FAILURE 1
#define NO_LINE -1

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Enum to declare error codes
Each error code has its own error message with paramters*/
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
    ERROR_LABEL_EMPTY_LINE,
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
    ERROR_MACRO_EMPTY_NAME,
    ERROR_MACRO_ALREADY_EXISTS,
    ERROR_MEMORY_EXCEEDED
} ErrorCode;

typedef enum
{
    IGNORED_LABEL
} WarningCode;

/*Struct to hold error code, message and parameter indicator.
used to search for error message based on the error code found*/
typedef struct
{
    ErrorCode code;
    char *message;
    char is_extra_word; /*If the error message has an extra parameter needed in the message*/
} Error;

typedef struct
{
    WarningCode code;
    char *message;
    char is_extra_word;
} Warning;

/*Struct to hold error code, extra word, and line number.
used to pass error information inside functions and handle errors throught the program*/
typedef struct
{
    ErrorCode code;
    char extra_word[MAX_LINE];
    int line_number;
} ErrorObject;

/*Function to handle errors.
Prints the error message based on the error code and extra paramter.*/
void handle_error(ErrorObject *error);
/*Function to fill an error object with error code, line number, and extra word.
Used to pass error information to the error handling function*/
int fill_error_object(ErrorCode code, int line_number, char *extra_word, ErrorObject *error);
/*Function to handle system errors.
Prints the error message based on the error code without specific line number.*/
void handle_system_error(ErrorCode code);
#endif