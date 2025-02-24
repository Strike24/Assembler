#include <stdio.h>
typedef enum
{
    FILE_NOT_FOUND = -1,
    WRONG_FILE_FORMAT = -2,

} ErrorType;

const char *error_messages[] = {
    "File not found",
    "Wrong file format",
};

void log_error(ErrorType error, int line_number, const char *file_name);