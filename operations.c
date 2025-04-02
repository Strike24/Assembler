#include "operations.h"

/*
 operations.c
 This file contains the implementation of the operation handling functions.
 It defines the operations table, and holds functions to validate operations, instructions and operands.
*/

/* Constant operations array that holds information for each code operation.
Table holds: name, opcode, funct, allowed source methods, allowed target methods*/
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
    /*Check if register name is valid*/
    if (register_name == NULL)
    {
        return -1;
    }

    if (register_name[0] != 'r')
    {
        return -1;
    }

    if (strlen(register_name) != REGISTER_NAME_LENGTH)
    {
        return -1;
    }

    if (register_name[1] < '0' || register_name[1] > '7')
    {
        return -1;
    }

    /*get number of register from 1 - 7*/
    return register_name[1] - '0';
}

int is_operation_name(char *name)
{
    Operation *operation = NULL;
    if (name == NULL)
    {
        return FALSE;
    }

    /*Check if name is a valid operation by searching in the operations table*/
    operation = get_operation(name);
    if (operation != NULL)
    {
        return TRUE;
    }
    return FALSE;
}

int is_register_name(char *name)
{
    if (name == NULL)
    {
        return FALSE;
    }

    /*Check if name is a valid register name by searching in the register table*/
    if (get_register_index(name) != -1)
    {
        return TRUE;
    }
    return FALSE;
}

Operation *get_operation(char *name)
{
    int i;
    if (name == NULL)
    {
        return NULL; /*Operation name is NULL*/
    }

    /*Search for the operation name in the operations table*/
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            return &OPERATIONS[i]; /*Return pointer to the operation details*/
        }
    }
    return NULL; /*Operation not found*/
}

OperandType *get_allowed_addressing_methods(char *name, int source_or_target)
{
    Operation *operation = NULL;
    if (name == NULL)
    {
        return NULL;
    }

    /*Get operation from the operations table*/
    operation = get_operation(name);
    if (operation == NULL)
    {
        return NULL; /*Operation not found*/
    }

    /*If source_or_target is SOURCE, return the allowed source methods. if target, return allowed target methods*/
    if (source_or_target == SOURCE)
    {
        return operation->allowed_source;
    }
    else if (source_or_target == TARGET)
    {
        return operation->allowed_target;
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
    if ((strcmp(word, DATA_INSTRUCTION) == 0) || (strcmp(word, STRING_INSTRUCTION) == 0))
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
    if (strcmp(word, EXTERN_INSTRUCTION) == 0)
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
    if (strcmp(word, ENTRY_INSTRUCTION) == 0)
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

    if (!word_original)
    {
        return FALSE;
    }

    /*Copy word to new string to avoid strtok changing the original*/
    word = (char *)malloc(strlen(word_original) + 1);
    strcpy(word, word_original);
    colon = strchr(word, ':');
    if (colon == NULL) /*Check if the ":" are existent*/
    {
        free(word);
        return FALSE;
    }
    /*Check if there is a space after the ":"*/
    if (*(colon + 1) == ' ')
    {
        free(word);
        return FALSE;
    }
    /*Remove the ":" from word*/
    *colon = '\0';

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
        /*If name is invalid, its still a valid declaration. fill error object but return true*/
    }

    free(word);
    return TRUE;
}

ErrorCode validate_label_name(char *word, MacroNode *macro_list)
{
    int i;
    if (!macro_list)
    {
        return ERROR_NULL_PARAM;
    }
    if (!word)
    {
        return ERROR_LABEL_EMPTY_NAME;
    }
    /*Check maximum length of label*/
    if (strlen(word) > MAX_LABEL)
    {
        return ERROR_LABEL_NAME_TOO_LONG;
    }

    if (isspace(word[0]) || word[0] == '\0')
    {
        return ERROR_LABEL_EMPTY_NAME;
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
        if (!isalnum(word[i])) /*If word[i] is not alphanumeric*/
        {
            if ((i == strlen(word) - 1) && (word[i] == '\n')) /*If last char is \n, its valid*/
                return TRUE;
            else
            {
                return ERROR_LABEL_NOT_ALPHANUMERIC;
            }
        }
    }
    /*Check if label is a reserved word*/
    if (is_reserved_word(word))
    {
        return ERROR_LABEL_RESERVED_WORD;
    }

    /*Check if label is already a macro name (macro names can't be used for labels)*/
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

    if ((strcmp(word, "mcro") == 0) || (strcmp(word, "mcroend") == 0))
        return TRUE;

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
    int i = 0;
    if (word == NULL)
    {
        return ERROR_NO_OPERAND;
    }

    /*Check if theres an extra comma at the start or end*/
    if (word[0] == ',' || word[strlen(word) - 1] == ',')
    {
        return ERROR_EXTRA_COMMA;
    }

    /*Check if there are two commas in a row*/
    for (i = 0; word[i] != '\0'; i++)
    {
        if (word[i] == ',' && word[i + 1] == ',')
        {
            return ERROR_EXTRA_COMMA;
        }
    }

    word = strtok(word, ","); /*Split the string by commas*/
    while (word != NULL)      /*For every operand seperated by a comma, check if valid number*/
    {

        if (!is_integer(word, strlen(word)))
        {
            return ERROR_INVALID_NUMBER;
        }

        word = strtok(NULL, ",");
    }

    /*If there is extra content after numbers*/
    if (word != NULL)
    {
        return ERROR_EXTRA_TEXT;
    }

    return SUCCESS;
}

ErrorCode validate_string(char *word)
{
    if (word == NULL)
    {
        return ERROR_NO_OPERAND;
    }
    /*Check if word is a string*/
    if (word[0] != '"')
    {
        return ERROR_INVALID_STRING;
    }
    word++;
    while (word[0] != '"') /*Loop until finding the end quotes. if found \0 before it, string doesn't end in quotes.*/
    {
        if (word[0] == '\0')
        {
            return ERROR_INVALID_STRING;
        }

        word++;
    }
    if (word[1] != '\0') /*Check if there is extra content after the end of the string*/
    {
        return ERROR_INVALID_STRING;
    }
    return SUCCESS;
}

int is_integer(char *str, int len)
{

    int i = 0;
    if (str == NULL)
        return 0;

    while ((i < len) && (str[0] == ' ' || str[0] == '\t')) /*Skip spaces at the start*/
    {
        str++;
        i++;
    }

    if ((i < len) && (str[0] == '+' || str[0] == '-')) /*Check if start is + or -, skip if found*/
    {
        str++;
        i++;
    }

    if ((i < len) && !isdigit(str[0])) /*Check if first char is digit*/
    {
        return FALSE;
    }

    while ((i < len) && isdigit(str[0])) /*Loop over rest of the chars, skip if found a digit*/
    {
        str++;
        i++;
    }

    while ((i < len) && (str[0] == ' ' || str[0] == '\t' || str[0] == '\n')) /*Skip spaces, tabs and new line at the end*/
    {
        str++;
        i++;
    }

    /*If str[0] is not \0, the string is not a number because we didn't skip all of the chars (which means they are invalid)*/
    return str[0] == '\0';
}

ErrorCode validate_extern(char *word, MacroNode *macro_list)
{
    ErrorCode result = SUCCESS;
    char *extra_content = NULL;
    word = strtok(word, " ");
    if (!macro_list)
    {
        return ERROR_NULL_PARAM;
    }
    if (!word)
    {
        /*No data found*/
        return ERROR_EXTERN_EMPTY_NAME;
    }

    extra_content = strtok(NULL, " ");
    if (extra_content != NULL)
    {
        /*Extra content found after extern label operation*/
        return ERROR_EXTRA_TEXT;
    }
    /*Check if operand is a valid label name*/
    result = validate_label_name(word, macro_list);
    return result;
}

ErrorCode validate_entry(char *word)
{
    char *extra_content = NULL;
    if (word == NULL)
    {
        return ERROR_NO_OPERAND;
    }

    word = strtok(word, " ");
    if (word == NULL)
    {
        /*No data found*/
        return ERROR_EXTERN_EMPTY_NAME;
    }

    extra_content = strtok(NULL, " ");
    if (extra_content != NULL)
    {
        /*Extra content found after entry label operation*/
        return ERROR_EXTRA_TEXT;
    }

    return SUCCESS;
}

ErrorCode validate_code(char *operation, char *oprands)
{

    OperandType *allowed_source_methods;
    OperandType *allowed_target_methods;
    OperandType current_oprand_type = NONE;
    int i = 0;
    int result = SUCCESS;
    if (operation == NULL)
    {
        return ERROR_NULL_PARAM;
    }

    allowed_source_methods = get_allowed_addressing_methods(operation, SOURCE); /*Get allowed source methods*/
    allowed_target_methods = get_allowed_addressing_methods(operation, TARGET); /*Get allowed target methods*/

    if (allowed_source_methods == NULL || allowed_target_methods == NULL)
    {
        return ERROR;
    }
    oprands = strtok(oprands, ","); /*Split the oprands by commas*/

    if (allowed_source_methods[0] != NONE) /*if source operands are allowed for operation*/
    {
        if (oprands == NULL)
        {
            /*Error: No oprands found, but needed for command*/
            return ERROR_INVALID_AMOUNT_OF_OPERANDS;
        }
        current_oprand_type = get_operand_type(oprands);
        if (current_oprand_type == NONE)
        {
            /*Error: Operand type is not defined*/
            return ERROR_INVALID_OPERAND_TYPE;
        }
        for (i = 0; allowed_source_methods[i] != END_OF_ARRAY; i++) /*Loop over allowed source types*/
        {
            if (allowed_source_methods[i] == current_oprand_type) /*If operand type is allowed, skip to next operand. mark as success*/
            {
                oprands = strtok(NULL, ",");
                result = SUCCESS;
                break;
            }
            result = ERROR;
        }
    }
    if (allowed_target_methods[0] != NONE) /*if target operands are allowed for operation*/
    {
        if (oprands == NULL)
        {
            /*Error: No oprands found, but needed for command*/
            return ERROR_INVALID_AMOUNT_OF_OPERANDS;
        }
        current_oprand_type = get_operand_type(oprands);
        if (current_oprand_type == NONE)
        {
            /*Error: Operand type is not defined*/
            return ERROR_INVALID_OPERAND_TYPE;
        }
        for (i = 0; allowed_target_methods[i] != END_OF_ARRAY; i++) /*Loop over allowed target types*/
        {
            if (allowed_target_methods[i] == current_oprand_type) /*If operand type is allowed, skip to next operand. mark as success*/
            {
                oprands = strtok(NULL, ",");
                result = SUCCESS;
                break;
            }
            result = ERROR;
        }
        oprands = strtok(NULL, ",");
    }

    if (oprands != NULL)
    {
        /*Error: Too many oprands*/
        return ERROR_INVALID_AMOUNT_OF_OPERANDS;
    }
    if (result == ERROR)
    {
        return ERROR_OPERAND_NOT_ALLOWED;
    }

    return result;
}

int get_operand_type(char *word)
{
    if (!word)
    {
        return NONE;
    }
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
    if (!word)
    {
        return FALSE;
    }

    if (word[0] == '#') /*Immidiate operands start with #*/
    {
        word++;
        if (is_integer(word, strlen(word))) /*Immediate operands are a valid integer*/
        {
            return TRUE;
        }
    }
    return FALSE;
}
int is_direct(char *word)
{
    if (!word)
    {
        return FALSE;
    }

    return TRUE;
}
int is_relative(char *word)
{
    if (!word)
    {
        return FALSE;
    }
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
    if (!word)
    {
        return FALSE;
    }
    if (is_register_name(word))
    {
        return TRUE;
    }
    return FALSE;
}