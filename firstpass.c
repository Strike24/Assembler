#include "firstpass.h"

int first_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, MacroNode *macro_list, int *IC, int *DC)
{
    FILE *input_file;
    char line[MAX_LINE];
    /* TODO: NOT SURE is_label IS REQUIRED HERE, CAN MAYBE BE ONLY IN parse_line*/
    int is_label = FALSE;
    int line_number = 1;
    int is_error = FALSE;
    ErrorObject error = {0};
    int memory_exceeded = FALSE;

    /*Open file after preassmbler macro expantion (.am)*/
    input_file = open_file(filename, "r", POST_MACRO_EXT);

    if (input_file == NULL)
    {
        /*EXITING AND MOVING TO NEXT FILE*/
        return ERROR;
    }

    /*Read line by line, send each line for parsing*/
    while (fgets(line, MAX_LINE, input_file))
    {
        /*if line is longer than 80 charcthers (not including \0) and fgets cut it, handle error*/
        if (line == NULL || line[0] == ';')
        {
            line_number++;
            continue;
        }
        else if (line[strlen(line) - 1] != '\n' && !feof(input_file))
        {

            fill_error_object(ERROR_LINE_TOO_LONG, line_number, MAX_LINE_STRING, &error);
            handle_error(&error);
            is_error = TRUE;

            /*Skip chars not read on file so fgets reads a new line*/
            while (fgetc(input_file) != '\n')
                ;
        }
        else
        {
            is_error = (parse_line(line, IC, DC, line_number, &is_label, &memory_exceeded, code_image, data_image, label_list, macro_list) || is_error);
        }
        line_number++;
    }

    /*Close file after all lines were read (reached EOF)*/
    /*print_binary_image(code_image);*/

    /*Update addresses for data labels by adding IC*/
    update_data_addresses(label_list, *IC);
    fclose(input_file);
    return is_error;
}

int parse_line(char *line, int *IC, int *DC, int line_number, int *is_label, int *memory_exceeded, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, MacroNode *macro_list)
{
    /*Save original line because we are changing "line" with strtok*/
    char line_original[MAX_LINE];
    char *word = line_original;
    BinaryLine binaryLine;
    /*validate_line will return the operation type for each line of code*/
    LabelType symbol = 0;
    ErrorObject error = {0};
    ErrorCode error_code = SUCCESS;
    /*Copy line to original line before making changes*/
    strcpy(line_original, line);

    /*Validate line and get operation type*/
    symbol = validate_line(line, &error, is_label, line_number, macro_list);

    /*If label was found, get the label name and skip to next word*/
    if (*is_label)
    {
        word = strtok(line_original, " \t\n");
        if (word == NULL)
        {
            fill_error_object(ERROR_LABEL_EMPTY_NAME, line_number, NULL, &error);
            handle_error(&error);
            is_label = FALSE;
            return TRUE;
        }

        word[strlen(word) - 1] = '\0'; /*Remove the ':' from the label name*/
        if (symbol == EXTERNAL || symbol == ENTRY)
        {
            printf("WARNING, Line: %d\nLabel cannot be decleared before .extern / .entry. \n\tLabel \"%s\" will be ignored by the assembler.", line_number, word);
        }
        else
        {
            if (symbol == CODE)
                error_code = add_label(label_list, word, *IC, symbol);
            else if (symbol == DATA)
            {
                error_code = add_label(label_list, word, *DC, symbol);
            }
            else if (symbol == INVALID_TYPE) /*WE DO NOT WANT ERROR ON THE LABEL, BECAUSE IT EXITS LEGALY*/
            {
                error_code = add_label(label_list, word, *IC, symbol);
            }
        }
        if (error_code != SUCCESS)
        {
            fill_error_object(error_code, line_number, word, &error);
            handle_error(&error);
            *is_label = FALSE;
            return FAILURE;
        }
        else
        {
            word = strtok(NULL, "");
            *is_label = FALSE;
        }
    }

    if (error.code == SUCCESS)
    {
        if (*IC + *DC >= MAX_MEMORY)
        {
            /*If memory exceeded maximum limit of 2^21, print and don't code the line*/
            if (*memory_exceeded == FALSE)
            {
                handle_system_error(ERROR_MEMORY_EXCEEDED);
            }
            *memory_exceeded = TRUE;
            return TRUE;
        }
        switch (symbol)
        {
        case DATA:
            binaryLine = data_binary(DC, word, line_number);
            add_binary_line(binaryLine, data_image);
            break;
        case EXTERNAL:
            add_extern_label_to_list(word, label_list);
            break;
        case CODE:
            binaryLine = code_binary(IC, word, line_number);
            add_binary_line(binaryLine, code_image);
            break;
        case ENTRY:
            /*.entry will be handled at the second pass.*/
            break;
        default:
            /*if no valid type, error code will not be success*/
            break;
        }
    }
    else
    {
        handle_error(&error);
        return TRUE;
    }

    return 0;
}
/*
void format_line(char *original_line, int original_len, char *formatted_line, int formatted_len)
{
    int i = 0;
    int j = 0;
    int is_space_found = FALSE;

    for (i = 0; (i < original_len) && (j < original_len); i++)
    {
        if (original_line[i] == '\n')
        {
            formatted_line[j] = original_line[i];
            j++;
        }
        if (original_line[i] == ':')
        {
            if (isspace(original_line[i + 1]))
            {
                formatted_line[j] = original_line[i];
                j++;
            }
            else
            {
                formatted_line[j] = original_line[i];
                j++;
                if (j + 1 < original_len)
                {
                    formatted_line[j + 1] = ' ';
                    j++;
                }
            }
        }
        else if (isspace(original_line[i]) && is_space_found == FALSE)
        {
            is_space_found = TRUE;
            formatted_line[j] = ' ';
            j++;
        }
        else
        {
            formatted_line[j] = original_line[i];
            j++;

            is_space_found = FALSE;
        }
    }
}
*/

LabelType validate_line(char *line, ErrorObject *error, int *is_label, int line_number, MacroNode *macro_list)
{
    char line_original[MAX_LINE];
    char *word;
    char *rest;
    int is_string = FALSE; /* data 0 string 1*/
    strcpy(line_original, line);
    /* format_line(line_original, strlen(line_original), line, strlen(line));*/
    /*Get first word in line*/
    word = strtok(line_original, " \t\n");
    if (is_label_dec(word, line_number, error, macro_list)) /*If label declaration found, turn on flag, skip to next word*/
    {
        if (error->code == SUCCESS)
        {
            *is_label = TRUE;
            word = strtok(NULL, " \t\n");
        }
        else
        {
            *is_label = FALSE;
            return INVALID_TYPE; /*Error will be handled later in parse line*/
        }
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
            fill_error_object(validate_string(word), line_number, word, error);
        }
        else
        {
            fill_error_object(validate_data(word), line_number, word, error);
        }

        return DATA;
    }

    else if (is_extern_operation(word))
    {
        word = strtok(NULL, "");
        trim(word);
        fill_error_object(validate_extern(word, macro_list), line_number, word, error);
        return EXTERNAL;
    }
    else if (is_entry_operation(word))
    {
        word = strtok(NULL, "");
        trim(word);
        fill_error_object(validate_entry(word), line_number, word, error);
        return ENTRY;
    }
    else if (is_code_operation(word))
    {
        /*For code, send the entire line, including the operation*/
        rest = strtok(NULL, "");
        trim(word);
        trim(rest);
        fill_error_object(validate_code(word, rest), line_number, word, error);
        return CODE;
    }
    else
    {
        if ((word == NULL) && *is_label)
        {
            fill_error_object(ERROR_LABEL_EMPTY_LINE, line_number, NULL, error);
        }
        else if (word == NULL)
        {
            return INVALID_TYPE; /*EMPTY LINE*/
        }
        else
        {
            fill_error_object(ERROR_INVALID_OPERATION_TYPE, line_number, word, error);
        }
        return INVALID_TYPE;
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