#include "firstpass.h"

int first_pass(char *filename)
{
    FILE *input_file;
    Label *label_list = init_label_table();
    int IC = 100;
    int DC = 0;
    char line[MAX_LINE];
    /* TODO: NOT SURE is_label IS REQUIRED HERE, CAN MAYBE BE ONLY IN parse_line*/
    int is_label = FALSE;

    /*Open file after preassmbler macro expantion (.am)*/
    input_file = open_file(filename, "r", POST_MACRO_EXT);

    if (input_file == NULL)
    {
        printf("Error: File not found\n");
        return ERROR;
    }

    /*Read line by line, send each line for parsing*/
    while (fgets(line, MAX_LINE, input_file))
    {
        parse_line(line, &IC, &DC, &is_label, label_list);
    }

    /*Close file after all lines were read (reached EOF)*/
    fclose(input_file);
    return 0;
}

int parse_line(char *line, int *IC, int *DC, int *is_label, Label *label_list)
{
    /*Save original line because we are changing "line" with strtok*/
    char line_original[MAX_LINE];
    /*validate_line will return the operation type for each line of code*/
    LabelType symbol = 0;
    int isValid = FALSE;
    /*Copy line to original line before making changes*/
    strcpy(line_original, line);

    /*Validate line and get operation type */
    symbol = validate_line(line, &isValid, is_label);
    /*For every symbol type, code operation and oprands into binary
    If label is present, add it to the symbols table*/
    switch (symbol)
    {
    case DATA:
        /* code */
        break;
    case EXTERNAL:
        /* code */
        break;
    case ENTRY:
        break;
    case CODE:
        /* code */
        break;

    default:
        break;
    }

    return TRUE;
}

LabelType validate_line(char *line, int *is_valid, int *is_label)
{
    char line_original[MAX_LINE];
    strcpy(line_original, line);
    /*Get first word in line*/
    char *word = strtok(line, " \t\n");
    if (is_label_dec(word)) /*If label declaration found, turn on flag, skip to next word*/
    {
        *is_label = TRUE;
        word = strtok(NULL, " \t\n");
    }

    /*Check if word is a valid operation, detrmain it's type*/
    if (is_data_operation(word))
    {
        /*Get rest of the line without the operation*/
        word = strtok(NULL, "");
        /*Check validation of operands and format*/
        *is_valid = validate_data(word);
        return DATA;
    }
    else if (is_extern_operation(word))
    {
        word = strtok(NULL, "");
        *is_valid = validate_extern(word);
        return EXTERNAL;
    }
    else if (is_entry_operation(word))
    {
        word = strtok(NULL, "");
        *is_valid = validate_entry(word);
        return ENTRY;
    }
    else if (is_code_operation(word))
    {
        /*No need to get rest of the line, need to send the entire line to validate_code*/
        *is_valid = validate_code(line_original);
        return CODE;
    }
    else
    {
        *is_valid = FALSE;
        return 0;
    }
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

    /*Check maximum length of label*/
    if (strlen(word) > 31)
    {
        free(word);
        return FALSE;
    }

    /*Check if label starts with alphabatic letter*/
    if (!isalpha(word[0]))
    {
        free(word);
        return FALSE;
    }

    /*Check if label letters are alphabatic or digits*/
    for (i = 1; (i < strlen(word)); i++)
    {
        if (!isalnum(word[i]))
        {
            free(word);
            return FALSE;
        }
    }
    /*Check if label is a reserved word*/
    if (is_reserved_word(word))
    {
        free(word);
        return FALSE;
    }

    free(word);
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
    word = strtok(word, ",");
    while (word != NULL)
    {
        if (word == NULL)
        {
            /*Error: No data found*/
            return FALSE;
        }

        if ((word[0] != '+') && (word[0] != '-') && (!isdigit(word[0])))
        {
            /*Error: word must start with +,- or digit*/
            return FALSE;
        }

        strtol(word, &endptr, BASE_10);
        if (endptr == word)
        {
            /*Error: no numbers were found*/
            return FALSE;
        }
        else if (*endptr != '\0')
        {
            /*Error: word is not a number, invalid chars found*/
            printf("%s\n", endptr);
            return FALSE;
        }

        word = strtok(NULL, ",");
    }
    return TRUE;
}

int validate_extern(char *word)
{
    word = strtok(word, " ");
    if (word == NULL)
    {
        /*Error: No data found*/
        return FALSE;
    }
    /*Otherwise, label name validation will be handled in the second pass where the label
    is already initalized*/
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
    /*Otherwise, label name validation will be handled in the second pass where the label
    is already initalized*/
    return TRUE;
}

int validate_code(char *word)
{
    word = strtok(word, " \t\n");
    OperandType *allowed_source_methods = get_allowed_addressing_methods(word, SOURCE);
    OperandType *allowed_target_methods = get_allowed_addressing_methods(word, TARGET);
}
