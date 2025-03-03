#include "operations.h"

Operation OPERATIONS[NUM_OF_OPERATIONS] = {
    {"mov", 0, 0, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"cmp", 1, 0, {IMMEDIATE, DIRECT, REGISTER}, {IMMEDIATE, DIRECT, REGISTER}},
    {"add", 2, 1, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"sub", 2, 2, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"lea", 4, 0, {DIRECT, IMMEDIATE}, {DIRECT, REGISTER}},
    {"clr", 5, 1, {0}, {DIRECT, REGISTER}},
    {"not", 5, 2, {0}, {DIRECT, REGISTER}},
    {"inc", 5, 3, {0}, {DIRECT, REGISTER}},
    {"dec", 5, 4, {0}, {DIRECT, REGISTER}},
    {"jmp", 9, 1, {0}, {DIRECT, RELATIVE}},
    {"bne", 9, 2, {0}, {DIRECT, RELATIVE}},
    {"jsr", 9, 3, {0}, {DIRECT, RELATIVE}},
    {"red", 12, 0, {0}, {DIRECT, REGISTER}},
    {"prn", 13, 0, {0}, {IMMEDIATE, DIRECT, REGISTER}},
    {"rts", 14, 0, {0}, {0}},
    {"stop", 15, 0, {0}, {0}}};

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

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
    int i;
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
        if (strcmp(register_names[i], name) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
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
    /*Check if word is .data or .string which is a data operation*/
    if ((strcmp(word, ".data") == 0) || (strcmp(word, ".string") == 0))
    {
        return TRUE;
    }
    return FALSE;
}

int is_extern_operation(char *word)
{
    /*Check if word is .extern which is an extern operation*/
    if (strcmp(word, ".extern") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int is_entry_operation(char *word)
{
    /*Check if word is .entry which is an entry operation*/
    if (strcmp(word, ".entry") == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int is_code_operation(char *word)
{
    /*Check if word is a valid code operation name*/
    /*Searches if word is found in the code operations table*/
    return is_operation_name(word);
}

int is_label_dec(char *word_original)
{
    /*Copy word to new string to avoid strtok changes*/
    char *word = (char *)malloc(strlen(word_original) + 1);
    char *colon;
    int i;

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
    if (!is_legal_label_name(word))
    {
        free(word);
        return FALSE;
    }

    free(word);
    return TRUE;
}

int is_legal_label_name(char *word)
{
    int i;
    /*Check maximum length of label*/
    if (strlen(word) > 31)
    {
        return FALSE;
    }

    /*Check if label starts with alphabatic letter*/
    if (!isalpha(word[0]))
    {
        return FALSE;
    }

    /*Check if label letters are alphabatic or digits*/
    for (i = 1; (i < strlen(word)); i++)
    {
        if (!isalnum(word[i]))
        {
            if ((i == strlen(word) - 1) && (word[i] == '\n'))
                return TRUE;
            else
                return FALSE;
        }
    }
    /*Check if label is a reserved word*/
    if (is_reserved_word(word))
    {
        return FALSE;
    }

    return TRUE;
}

int is_reserved_word(char *word)
{
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

int validate_data(char *word)
{
    char *endptr; /*Used for strtol to check if word is a number*/
    int res = FALSE;
    word = strtok(word, ",");
    while (word != NULL)
    {
        res = TRUE;
        /*
        if ((word[0] != '+') && (word[0] != '-') && (!isdigit(word[0])) && (word[0] != ' ') && (word[0] != '\t'))
        {
            return FALSE;
        }

        strtol(word, &endptr, BASE_10);
        if (endptr == word)
        {
            return FALSE;
        }
        else if (*endptr != '\0')
        {
            printf("%s\n", endptr);
            return FALSE;
        }
        */

        if (!is_integer(word, strlen(word)))
        {
            res = FALSE;
            return res;
        }

        word = strtok(NULL, ",");
    }
    return res;
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

int validate_extern(char *word)
{
    word = strtok(word, " ");
    if (word == NULL)
    {
        /*Error: No data found*/
        return FALSE;
    }

    if (!is_legal_label_name(word))
    {
        return FALSE;
    }
    return TRUE;
}
int validate_entry(char *word)
{
    word = strtok(word, " ");
    if (word == NULL)
    {
        /*Error: No data found*/
        return FALSE;
    }

    if (!is_legal_label_name(word))
    {
        return FALSE;
    }

    return TRUE;
}

int validate_code(char *operation, char *oprands)
{

    OperandType *allowed_source_methods;
    OperandType *allowed_target_methods;
    allowed_source_methods = get_allowed_addressing_methods(operation, SOURCE);
    allowed_target_methods = get_allowed_addressing_methods(operation, TARGET);
    oprands = strtok(oprands, ",");
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
    if (is_legal_label_name(word))
    {
        return TRUE;
    }
    return FALSE;
}
int is_relative(char *word)
{
    if (word[0] == '&')
    {
        word++;
        if (is_legal_label_name(word))
        {
            return TRUE;
        }
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