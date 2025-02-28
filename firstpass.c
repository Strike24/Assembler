#include "firstpass.h"

int first_pass(char *filename)
{
    FILE *input_file;
    Label *label_list = init_label_table();
    int IC = 100;
    int DC = 0;
    char line[MAX_LINE];
    int is_label = FALSE;

    input_file = open_file(filename, "r", POST_MACRO_EXT);

    if (input_file == NULL)
    {
        printf("Error: File not found\n");
        return ERROR;
    }

    while (fgets(line, MAX_LINE, input_file))
    {
        parse_line(line, &IC, &DC, &is_label, label_list);
    }

    fclose(input_file);
    return 0;
}

int parse_line(char *line, int *IC, int *DC, int *is_label, Label *label_list)
{
    /*char *word;*/
    char line_original[MAX_LINE];
    LabelType symbol = 0;
    int isValid = FALSE;
    /*Copy line to original line before making changes*/
    strcpy(line_original, line);

    symbol = validate_line(line, &isValid, is_label);

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
    char *word = strtok(line, " \t\n");
    if (is_label_dec(word)) /*If label is found, turn on flag, skip to next word*/
    {
        *is_label = TRUE;
        word = strtok(NULL, " \t\n");
    }

    if (is_data_operation(word))
    {
        word = strtok(NULL, "");
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
        word = strtok(NULL, "");
        *is_valid = validate_code(word);
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

    if (strcmp(word, ".data") || strcmp(word, ".string"))
    {
        return TRUE;
    }
    return FALSE;
}

int is_extern_operation(char *word)
{
    if (strcmp(word, ".extern"))
    {
        return TRUE;
    }
    return FALSE;
}

int is_entry_operation(char *word)
{
    if (strcmp(word, ".entry"))
    {
        return TRUE;
    }
    return FALSE;
}

int is_code_operation(char *word)
{
    return is_operation_name(word);
}

int is_label_dec(char *word_original)
{
    char *word = (char *)malloc(strlen(word_original) + 1);
    int result = TRUE;
    char *colon;
    int i;

    strcpy(word, word_original);
    colon = strchr(word, ':');
    if (colon == NULL) /*Check if : are existent*/
        result = FALSE;
    /*Remove : from word*/
    *colon = '\0';

    /*Check if label is valid*/
    /*Check that there were no spaces before :*/
    if (word[strlen(word) - 1] == ' ')
        result = FALSE;

    /*Check maximum length of label*/
    if (strlen(word) > 31)
        result = FALSE;

    /*Check if label starts with alphabatic letter*/
    if (!isalpha(word[0]))
        result = FALSE;

    /*Check if label letters are alphabatic or digits*/
    for (i = 1; (i < strlen(word)) && (result == TRUE); i++)
    {
        if (!isalnum(word[i]))
        {
            result = FALSE;
        }
    }
    /*Check if label is a reserved word*/
    if (is_reserved_word(word))
        result = FALSE;

    free(word);
    return result;
}

int is_reserved_word(char *word)
{
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

    /*Check if equals to entry, data, extern without the .*/
    if (strcmp(word, "entry") || strcmp(word, "data") || strcmp(word, "extern"))
        return TRUE;

    return FALSE;
}

int validate_data(char *word) { return TRUE; }
int validate_extern(char *word) { return TRUE; }
int validate_entry(char *word) { return TRUE; }
int validate_code(char *word) { return TRUE; }
