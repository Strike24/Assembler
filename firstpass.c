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
        return ERROR;
    }

    /*Read line by line, send each line for parsing*/
    while (fgets(line, MAX_LINE, input_file))
    {
        /*if line is empty or a comment, skip it*/
        if (line == NULL || line[0] == ';')
        {
            line_number++;
            continue;
        }
        else if (line[strlen(line) - 1] != '\n' && !feof(input_file))
        {
            /*If line is too long - fgets will cut it before the \n
            , print error and skip to next line*/
            fill_error_object(ERROR_LINE_TOO_LONG, line_number, MAX_LINE_STRING, &error);
            handle_error(&error);
            is_error = TRUE;

            /*If fgets cuts it, it will read the rest of the line on the next loop.
            skip chars not read on file so fgets reads a completly new line*/
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
        /*Find first word in line, which is the label name*/
        word = strtok(line_original, " \t\n");
        if (word == NULL)
        {
            /*If no label name was found, return error*/
            fill_error_object(ERROR_LABEL_EMPTY_NAME, line_number, NULL, &error);
            handle_error(&error);
            is_label = FALSE;
            return TRUE;
        }

        word[strlen(word) - 1] = '\0'; /*Remove the ':' from the label decleration*/
        if (symbol == EXTERNAL || symbol == ENTRY)
        {
            /*If line is an extern or entry instruction, labels decleared before it will be ignored by the assembler*/
            printf("WARNING, Line: %d\nLabel cannot be decleared before .extern / .entry. \n\tLabel \"%s\" will be ignored by the assembler.", line_number, word);
        }
        else
        {
            /*If code operation line, add the label with the IC address. If data, add with the DC address*/
            if (symbol == CODE)
                error_code = add_label(label_list, word, *IC, symbol);
            else if (symbol == DATA)
            {
                error_code = add_label(label_list, word, *DC, symbol);
            }
            else if (symbol == INVALID_TYPE) /*Preventing undefined label errors when the label is valid but the line after is not.*/
            {
                error_code = add_label(label_list, word, *IC, symbol);
            }
        }

        /*TODO: NOT SURE THIS CAN EVEN HAPPEN, MAYBE UNNECCERY*/
        if (error_code != SUCCESS)
        {
            /*If error was found while adding label, print error and skip to next line*/
            fill_error_object(error_code, line_number, word, &error);
            handle_error(&error);
            *is_label = FALSE;
            return TRUE;
        }
        else
        {
            /*If label was added successfully, skip to next word*/
            word = strtok(NULL, "");
            *is_label = FALSE;
        }
    }

    /*If no error was found while validating the line, code it to binary and save it*/
    if (error.code == SUCCESS)
    {
        if (*IC + *DC >= MAX_MEMORY)
        {
            /*If memory exceeded maximum limit of 2^21, print error and don't code the line*/
            /*From now on the assembler will only do error validation without coding the lines*/
            if (*memory_exceeded == FALSE)
            {
                handle_system_error(ERROR_MEMORY_EXCEEDED);
            }
            *memory_exceeded = TRUE;
            return TRUE;
        }

        /*TODO: MAYBE CHANGE code_binary AND data_binary to work with pointers instead of by value*/
        /*Code line to binary and save it to the binary image*/
        switch (symbol)
        {
        case DATA:
            /*If line is a data line, code it to binary and save it to the data image*/
            binaryLine = data_binary(DC, word, line_number);
            add_binary_line(binaryLine, data_image);
            break;
        case EXTERNAL:
            /*If line is an extern line, add the label to the extern list*/
            add_extern_label_to_list(word, label_list);
            break;
        case CODE:
            /*If line is a code line, code it to binary and save it to the code image*/
            binaryLine = code_binary(IC, word, line_number);
            add_binary_line(binaryLine, code_image);
            break;
        case ENTRY:
            /*.entry will be handled at the second pass.*/
            break;
        default:
            break;
        }
    }
    else
    {
        /*If error was found while validating the line, print error and skip to next line*/
        handle_error(&error);
        return TRUE;
    }

    /*If no error was found, return 0*/
    return 0;
}

LabelType validate_line(char *line, ErrorObject *error, int *is_label, int line_number, MacroNode *macro_list)
{
    /* TODO: VALIDATE WRONG ,*/
    char line_original[MAX_LINE];
    char *word;
    char *rest;
    int is_string = FALSE;       /* data 0 string 1*/
    strcpy(line_original, line); /*Copy line to original line before making changes with strtok*/

    /*LABEL VALIDATION*/
    /*Get first word in line*/
    word = strtok(line_original, " \t\n");
    if (is_label_dec(word, line_number, error, macro_list))
    {
        if (error->code == SUCCESS)
        {
            /*If label declaration was found, and label name is valid - turn on flag and move to next word (operation / instruction)*/
            *is_label = TRUE;
            word = strtok(NULL, " \t\n");
        }
        else
        {
            /*If label declaration was found but label name is invalid*/
            *is_label = FALSE;
            return INVALID_TYPE; /*Error will be handled later in parse line*/
        }
    }

    /*Operation / Instruction VALIDATION*/
    /*Check if word is a valid operation or instruction, detrmain it's type*/
    /*Validate format and allowed input for each type*/
    if (is_data_operation(word))
    {
        if (strcmp(word, ".string") == 0)
        {
            is_string = TRUE; /*Set flag for string*/
        }
        /*Get rest of the line without the .data or .string*/
        word = strtok(NULL, "");
        trim(word); /*Remove leading and trailing whitespace*/

        /*Check validation of operands and format, fill the error object if theres an error*/
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
        /*Get rest of the line without the .extern*/
        word = strtok(NULL, "");
        trim(word);
        /*Check validation of operand (extern label name) and format, fill the error object if theres an error*/
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
        /*Detect the code operation, validate the format and operands*/
        fill_error_object(validate_code(word, rest), line_number, word, error);
        return CODE;
    }
    else
    {
        /*If line is empty after label but label declartion found*/
        if ((word == NULL) && *is_label)
        {
            /*Label cannot be delcared on an empty line*/
            fill_error_object(ERROR_LABEL_EMPTY_LINE, line_number, NULL, error);
        }
        else if (word == NULL) /*Line is empty, no label found*/
        {
            return INVALID_TYPE;
        }
        else /*Line is not a known operation or instruction type*/
        {
            fill_error_object(ERROR_INVALID_OPERATION_TYPE, line_number, word, error);
        }
        return INVALID_TYPE;
    }
}

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
        if (!isspace(str[i])) /*if not a whitespace, copy as is*/
        {
            str[j] = str[i];
            j++;
        }
        /*If whitespace, skip it*/
        i++;
    }
    /*Add null terminator to the end of the string*/
    str[j] = '\0';
}