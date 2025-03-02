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
    char *word;
    strcpy(line_original, line);
    /*Get first word in line*/
    word = strtok(line, " \t\n");
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
