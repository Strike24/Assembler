#include "operations.h"

Operation OPERATIONS[NUM_OF_OPERATIONS] = {
    {"mov", 0, 0, {IMMEDIATE, DIRECT, REGISTER, -1}, {DIRECT, REGISTER, -1}},
    {"cmp", 1, 0, {IMMEDIATE, DIRECT, REGISTER, -1}, {IMMEDIATE, DIRECT, REGISTER, -1}},
    {"add", 2, 1, {IMMEDIATE, DIRECT, REGISTER, -1}, {DIRECT, REGISTER, -1}},
    {"sub", 2, 2, {IMMEDIATE, DIRECT, REGISTER, -1}, {DIRECT, REGISTER, -1}},
    {"lea", 4, 0, {DIRECT, IMMEDIATE, -1}, {DIRECT, REGISTER, -1}},
    {"clr", 5, 1, {NONE}, {DIRECT, REGISTER, -1}},
    {"not", 5, 2, {NONE}, {DIRECT, REGISTER, -1}},
    {"inc", 5, 3, {NONE}, {DIRECT, REGISTER, -1}},
    {"dec", 5, 4, {NONE}, {DIRECT, REGISTER, -1}},
    {"jmp", 9, 1, {NONE}, {DIRECT, RELATIVE, -1}},
    {"bne", 9, 2, {NONE}, {DIRECT, RELATIVE, -1}},
    {"jsr", 9, 3, {NONE}, {DIRECT, RELATIVE, -1}},
    {"red", 12, 0, {NONE}, {DIRECT, REGISTER, -1}},
    {"prn", 13, 0, {NONE}, {IMMEDIATE, DIRECT, REGISTER, -1}},
    {"rts", 14, 0, {NONE}, {NONE}},
    {"stop", 15, 0, {NONE}, {NONE}}};

int get_register_index(char *register_name)
{
    int i;
    char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
        if (strcmp(register_names[i], register_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int is_operation_name(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int is_register_name(char *name)
{
    if (get_register_index(name) != -1)
    {
        return TRUE;
    }
    return FALSE;
}

Operation *get_operation(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            return &OPERATIONS[i];
        }
    }
    return NULL;
}

OperandType *get_allowed_addressing_methods(char *name, int source_or_target)
{
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            if (source_or_target == SOURCE)
            {
                return OPERATIONS[i].allowed_source;
            }
            else if (source_or_target == TARGET)
            {
                return OPERATIONS[i].allowed_target;
            }
        }
    }
    return NULL;
}

int is_data_operation(char *word)
{
    if (word == NULL)
    {
        return FALSE;
    }
    /*Check if word is .data or .string which is a data operation*/
    if ((strcmp(word, ".data") == 0) || (strcmp(word, ".string") == 0))
    {
        return TRUE;
    }
    return FALSE;
}

int is_extern_operation(char *word)
{
    if (word == NULL)
    {
        return FALSE;
    }
    /*Check if word is .extern which is an extern operation*/
    if (strcmp(word, ".extern") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int is_entry_operation(char *word)
{
    if (word == NULL)
    {
        return FALSE;
    }
    /*Check if word is .entry which is an entry operation*/
    if (strcmp(word, ".entry") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int is_code_operation(char *word)
{
    if (word == NULL)
    {
        return FALSE;
    }
    /*Check if word is a valid code operation name*/
    /*Searches if word is found in the code operations table*/
    return is_operation_name(word);
}

int is_label_dec(char *word_original, int line_number, ErrorObject *error, MacroNode *macro_list)
{
    char *word = NULL;
    char *colon = NULL;
    ErrorCode result = SUCCESS;

    if (word_original == NULL)
    {
        return FALSE;
    }
    /*Copy word to new string to avoid strtok changes*/
    word = (char *)malloc(strlen(word_original) + 1);
    strcpy(word, word_original);
    colon = strchr(word, ':');
    if (colon == NULL) /*Check if : are existent*/
    {
        free(word);
        return FALSE;
    }
    /*Remove : from word*/
    *colon = '\0';

    /*Check if label is valid*/
    /*Check that there were no spaces before :*/
    if (word[strlen(word) - 1] == ' ')
    {
        free(word);
        return FALSE;
    }

    /*Check if label name is valid*/
    result = validate_label_name(word, macro_list);
    if (result != SUCCESS)
    {
        if (error)
            fill_error_object(result, line_number, word, error);
    }

    free(word);
    return TRUE;
}

ErrorCode validate_label_name(char *word, MacroNode *macro_list)
{
    int i;
    if (word == NULL)
    {
        /* handle_line_error(ERROR_LABEL_EMPTY_NAME, line_number, word);
        return FALSE; */
        return ERROR_LABEL_EMPTY_NAME;
    }
    /*Check maximum length of label*/
    if (strlen(word) > MAX_LABEL)
    {
        /*  handle_line_error(ERROR_LABEL_NAME_TOO_LONG, line_number, word);
         return FALSE; */
        return ERROR_LABEL_NAME_TOO_LONG;
    }

    /*Check if label starts with alphabatic letter*/
    if (!isalpha(word[0]))
    {
        /* handle_line_error(ERROR_LABEL_INVALID_START, line_number, word);
        return FALSE; */
        return ERROR_LABEL_INVALID_START;
    }

    /*Check if label letters are alphabatic or digits*/
    for (i = 1; (i < strlen(word)); i++)
    {
        if (!isalnum(word[i]))
        {
            if ((i == strlen(word) - 1) && (word[i] == '\n'))
                return TRUE;
            else
            {
                /*  handle_line_error(ERROR_LABEL_NOT_ALPHANUMERIC, line_number, word);
                 return FALSE; */
                return ERROR_LABEL_NOT_ALPHANUMERIC;
            }
        }
    }
    /*Check if label is a reserved word*/
    if (is_reserved_word(word))
    {
        /*  handle_line_error(ERROR_LABEL_RESERVED_WORD, line_number, word);
         return FALSE; */
        return ERROR_LABEL_RESERVED_WORD;
    }

    /*Check if label is a macro name*/
    if (is_macro(word, macro_list))
    {
        return ERROR_LABEL_IS_MACRO_NAME;
    }

    return SUCCESS;
}

int is_reserved_word(char *word)
{
    if (word == NULL)
    {
        return FALSE;
    }
    /*Check if word is a reserved word by checking if it's a code, data, extern,
     entry operation or a register name \ macro name*/
    if (is_code_operation(word))
        return TRUE;

    if (is_data_operation(word))
        return TRUE;

    if (is_extern_operation(word))
        return TRUE;

    if (is_entry_operation(word))
        return TRUE;

    /*   if (is_macro(word))
        return TRUE;
        */

    if (is_register_name(word))
        return TRUE;

    /*Check if equals to entry, data, extern without the .
    they are also reserved words*/
    if ((strcmp(word, "entry") == 0) || (strcmp(word, "data") == 0) || (strcmp(word, "extern") == 0))
        return TRUE;

    return FALSE;
}

ErrorCode validate_data(char *word)
{
    if (word == NULL)
    {
        return ERROR_NULL_PARAM;
    }
    word = strtok(word, ",");
    while (word != NULL)
    {

        if (!is_integer(word, strlen(word)))
        {
            /*
            handle_line_error(ERROR_INVALID_NUMBER, line_number, word);
            res = FALSE;
            return res;
            */
            return ERROR_INVALID_NUMBER;
        }

        word = strtok(NULL, ",");
    }
    return SUCCESS;
}

ErrorCode validate_string(char *word)
{
    if (word == NULL)
    {
        return ERROR_INVALID_OPERAND_TYPE;
    }
    /*Check if word is a string*/
    if (word[0] != '"')
    {
        /*
        handle_line_error(ERROR_INVALID_STRING, line_number, word);
        return FALSE;
        */
        return ERROR_INVALID_STRING;
    }
    word++;
    while (word[0] != '"')
    {
        if (word[0] == '\0')
        {
            /*
            handle_line_error(ERROR_INVALID_STRING, line_number, word);
            return FALSE;
            */
            return ERROR_INVALID_STRING;
        }

        word++;
    }
    return SUCCESS;
}

int is_integer(char *str, int len)
{

    int i = 0;
    if (str == NULL)
        return 0;

    while ((i < len) && (str[0] == ' ' || str[0] == '\t'))
    {
        str++;
        i++;
    }

    if ((i < len) && (str[0] == '+' || str[0] == '-'))
    {
        str++;
        i++;
    }

    if ((i < len) && !isdigit(str[0]))
    {
        return FALSE;
    }

    while ((i < len) && isdigit(str[0]))
    {
        str++;
        i++;
    }

    while ((i < len) && (str[0] == ' ' || str[0] == '\t' || str[0] == '\n'))
    {
        str++;
        i++;
    }

    return str[0] == '\0';
}

ErrorCode validate_extern(char *word)
{
    word = strtok(word, " ");
    if (word == NULL)
    {
        /*No data found*/
        return ERROR_NULL_PARAM;
    }

    /*
    if (!is_legal_label_name(word, line_number))
    {
        return FALSE;
    }
        */
    return SUCCESS;
}
ErrorCode validate_entry(char *word)
{
    word = strtok(word, " ");
    if (word == NULL)
    {
        /*No data found*/
        return FAILURE;
    }

    /*
    if (!is_legal_label_name(word))
    {
        return FALSE;
    }
        */

    return SUCCESS;
}

ErrorCode validate_code(char *operation, char *oprands)
{

    OperandType *allowed_source_methods;
    OperandType *allowed_target_methods;
    OperandType current_oprand_type = NONE;
    int i = 0;
    int result = SUCCESS;

    allowed_source_methods = get_allowed_addressing_methods(operation, SOURCE);
    allowed_target_methods = get_allowed_addressing_methods(operation, TARGET);

    if (allowed_source_methods == NULL || allowed_target_methods == NULL)
    {
        return ERROR;
    }
    oprands = strtok(oprands, ",");

    if (allowed_source_methods[0] != NONE)
    {
        if (oprands == NULL)
        {
            /*Error: No oprands found, but needed for command*/
            /*
            handle_line_error(ERROR_INVALID_AMOUNT_OF_OPERANDS, line_number, operation);
            return FALSE;
            */
            return ERROR_INVALID_AMOUNT_OF_OPERANDS;
        }
        current_oprand_type = get_operand_type(oprands);
        if (current_oprand_type == -1)
        {
            /*Error: Operand type is not defined*/
            /*
            handle_line_error(ERROR_INVALID_OPERAND_TYPE, line_number, oprands);
            return FALSE;
            */
            return ERROR_INVALID_OPERAND_TYPE;
        }
        for (i = 0; allowed_source_methods[i] != -1; i++)
        {
            if (allowed_source_methods[i] == current_oprand_type)
            {
                oprands = strtok(NULL, ",");
                result = SUCCESS;
                break;
            }
            result = ERROR;
        }
    }
    if (allowed_target_methods[0] != NONE)
    {
        if (oprands == NULL)
        {
            /*Error: No oprands found, but needed for command*/
            /* handle_line_error(ERROR_INVALID_AMOUNT_OF_OPERANDS, line_number, operation);
            return FALSE; */
            return ERROR_INVALID_AMOUNT_OF_OPERANDS;
        }
        current_oprand_type = get_operand_type(oprands);
        if (current_oprand_type == -1)
        {
            /*Error: Operand type is not defined*/
            /* handle_line_error(ERROR_INVALID_OPERAND_TYPE, line_number, oprands);
            return FALSE; */
            return ERROR_INVALID_OPERAND_TYPE;
        }
        for (i = 0; allowed_target_methods[i] != -1; i++)
        {
            if (allowed_target_methods[i] == current_oprand_type)
            {
                oprands = strtok(NULL, ",");
                result = SUCCESS;
                break;
            }
            result = ERROR;
            oprands = strtok(NULL, ",");
        }
    }

    if (oprands != NULL)
    {
        /*Error: Too many oprands*/
        /* handle_line_error(ERROR_INVALID_AMOUNT_OF_OPERANDS, line_number, operation);
        return FALSE; */
        return ERROR_INVALID_AMOUNT_OF_OPERANDS;
    }
    /*
        if (result == FALSE)
        {
            handle_line_error(ERROR_OPERAND_NOT_ALLOWED, line_number, operation);
        } */
    if (result == ERROR)
    {
        return ERROR_OPERAND_NOT_ALLOWED;
    }

    return result;
}

int get_operand_type(char *word)
{
    if (is_immediate(word))
    {
        return IMMEDIATE;
    }
    else if (is_register(word))
    {
        return REGISTER;
    }
    else if (is_relative(word))
    {
        return RELATIVE;
    }
    else if (is_direct(word))
    {
        return DIRECT;
    }
    return NONE;
}

int is_immediate(char *word)
{
    if (word[0] == '#')
    {
        word++;
        if (is_integer(word, strlen(word)))
        {
            return TRUE;
        }
    }
    return FALSE;
}
int is_direct(char *word)
{
    /*
    if (is_legal_label_name(word))
    {
        return TRUE;
    }
        */

    return TRUE;
}
int is_relative(char *word)
{
    if (word[0] == '&')
    {
        /*
        word++;
        if (is_legal_label_name(word))
        {
            return TRUE;
        }
            */
        return TRUE;
    }
    return FALSE;
}
int is_register(char *word)
{
    if (is_register_name(word))
    {
        return TRUE;
    }
    return FALSE;
}