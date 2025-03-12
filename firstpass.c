#include "firstpass.h"

int first_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list)
{
    FILE *input_file;
    int IC = 100;
    int DC = 0;
    char line[MAX_LINE];
    /* TODO: NOT SURE is_label IS REQUIRED HERE, CAN MAYBE BE ONLY IN parse_line*/
    int is_label = FALSE;
    int line_number = 1;

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
        parse_line(line, &IC, &DC, line_number, &is_label, code_image, data_image, label_list);
        line_number++;
    }

    /*Close file after all lines were read (reached EOF)*/
    /*print_binary_image(code_image);*/

    /*Update addresses for data labels by adding IC*/
    update_data_addresses(label_list, IC);
    fclose(input_file);
    return 0;
}

int parse_line(char *line, int *IC, int *DC, int line_number, int *is_label, BinaryNode *code_image, BinaryNode *data_image, Label *label_list)
{
    /*Save original line because we are changing "line" with strtok*/
    char line_original[MAX_LINE];
    char *word = line_original;
    BinaryLine binaryLine;
    /*validate_line will return the operation type for each line of code*/
    LabelType symbol = 0;
    int isValid = FALSE;
    /*Copy line to original line before making changes*/
    strcpy(line_original, line);

    if (line[0] == ';') /*If line is a comment, skip it*/
    {
        return TRUE;
    }

    /*Validate line and get operation type*/
    symbol = validate_line(line, &isValid, is_label);

    /*If label was found, get the label name and skip to next word*/
    if (*is_label)
    {
        word = strtok(line_original, " \t\n");
        word[strlen(word) - 1] = '\0';
        if (symbol == EXTERNAL || symbol == ENTRY)
        {
            printf("Warning: Label cannot be decleared before .extern / .entry.\nLabel will be ignored.\n");
            printf("\t%s\n", line);
        }
        else
        {
            add_label(label_list, word, *IC, symbol);
        }
        word = strtok(NULL, "");
        *is_label = FALSE;
    }

    if (isValid)
    {
        switch (symbol)
        {
        case DATA:
            binaryLine = data_binary(DC, word, line_number);
            add_binary_line(binaryLine, data_image);
            break;
        case EXTERNAL:
            add_extern_label(word, label_list);
            break;
        case CODE:
            binaryLine = code_binary(IC, word, line_number);
            add_binary_line(binaryLine, code_image);
            break;
        case ENTRY:
            /*.entry will be handled at the second pass.*/
            break;
        default:
            printf("Error: Invalid line format\n\t%s\n", line);
            break;
        }
    }
    else
    {
        printf("Error: Invalid line format\n\t%s\n", line);
    }

    return TRUE;
}

LabelType validate_line(char *line, int *is_valid, int *is_label)
{
    char line_original[MAX_LINE];
    char *word;
    char *rest;
    int is_string = FALSE; /* data 0 string 1*/
    strcpy(line_original, line);
    /*Get first word in line*/
    word = strtok(line_original, " \t\n");
    if (is_label_dec(word)) /*If label declaration found, turn on flag, skip to next word*/
    {
        *is_label = TRUE;
        word = strtok(NULL, " \t\n");
    }

    /*Check if word is a valid operation, detrmain it's type*/
    /*Validate format and allowed input for each type of operation*/
    if (is_data_operation(word))
    {
        if (strcmp(word, ".string") == 0)
        {
            is_string = TRUE; /*Set flag for string*/
        }
        /*Get rest of the line without the operation*/
        word = strtok(NULL, "");
        trim(word);
        /*Check validation of operands and format*/

        if (is_string)
        {
            *is_valid = validate_string(word);
        }
        else
        {
            *is_valid = validate_data(word);
        }
        return DATA;
    }
    else if (is_extern_operation(word))
    {
        word = strtok(NULL, "");
        trim(word);
        *is_valid = validate_extern(word);
        return EXTERNAL;
    }
    else if (is_entry_operation(word))
    {
        word = strtok(NULL, "");
        trim(word);
        *is_valid = validate_entry(word);
        return ENTRY;
    }
    else if (is_code_operation(word))
    {
        /*For code, send the entire line, including the operation*/
        rest = strtok(NULL, "");
        trim(word);
        trim(rest);
        *is_valid = validate_code(word, rest);
        return CODE;
    }
    else
    {
        *is_valid = FALSE;
        return ERROR;
    }
}
/*trim all whitespaces from char* */
void trim(char *str)
{
    int i = 0;
    int j = 0;
    if (str == NULL)
    {
        return;
    }

    /*Loop to remove whitespace characters*/
    while (str[i] != '\0')
    {
        if (!isspace(str[i]))
        {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
}