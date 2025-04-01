#include "preasm.h"

int pre_assembler(char *filename, MacroNode *head)
{
    FILE *input_file, *output_file;

    if (!filename || !head)
    {
        return ERROR_NULL_PARAM;
    }

    /*Open the .as file for reading and create the .am file for writing*/
    input_file = open_file(filename, "r", PRE_MACRO_EXT);
    output_file = open_file(filename, "w", POST_MACRO_EXT);

    /*If either file failed to open, return an error and move to next file*/
    if (input_file == NULL || output_file == NULL)
    {
        return ERROR;
    }

    /*Expand macros in the .as file and write the expanded content to the .am file*/
    if (macro_expansion(input_file, output_file, head) == ERROR)
    {
        printf("Error: Macro expansion failed\n");
        fclose(input_file);
        fclose(output_file);
        return ERROR;
    }

    fclose(input_file);
    fclose(output_file);
    return SUCCESS;
}

int macro_expansion(FILE *input_file, FILE *output_file, MacroNode *head)
{
    char line[MAX_LINE] = {0};      /*Buffer to hold each line in the file (fgets)*/
    char line_copy[MAX_LINE] = {0}; /*Copy of the line buffer to be used because strtok changes line*/
    char *word = NULL;              /*Pointer to the current word in the line*/
    int in_macro_creation = FALSE;  /*Flag to indicate if the current line is inside a macro creation*/
    Macro *current_macro = NULL;    /*Pointer to the current macro being created*/
    ErrorObject error = {0};        /*Error object to be filled and passed to handle_error*/
    int line_number = 0;
    int is_error = FALSE;

    if (!input_file || !output_file || !head)
    {
        return ERROR_NULL_PARAM;
    }

    /*Loop over every line in the input file*/
    while (fgets(line, MAX_LINE, input_file))
    {
        line_number++;
        /*Copy the line to be used by strtok and keeping the original*/
        strcpy(line_copy, line);
        word = strtok(line, " \t\n");

        /*If the line is empty or a comment, write it to the output file as is*/
        if (!word || word[0] == ';')
        {
            fprintf(output_file, "%s", line_copy);
            continue;
        }

        if (!in_macro_creation)
        {
            process_line(head, output_file, word, &in_macro_creation, line_copy, &error);
            if (error.code != SUCCESS)
            {
                /*If there was an error in the macro expansion, return an error*/
                error.line_number = line_number;
                handle_error(&error);
                is_error = TRUE;
                continue;
            }
        }
        else
        {
            /*The line is inside a macro creation, get the current macro being created (last one added)*/
            current_macro = get_current_macro(head);
            /*If the macro was not added, or content space was not allocated, return an error and move to the next line*/
            if (!current_macro && !current_macro->content)
            {
                fill_error_object(ERROR_MACRO_NOT_DEFINED, line_number, NULL, &error);
                handle_error(&error);
                is_error = TRUE;
                continue;
            }

            /*If the word is the end of the macro creation, set the flag to indicate that we are no longer in macro creation*/
            /*do not print this line to the file*/
            if (strcmp(word, MACRO_DECLERATION_SUFFIX) == 0)
            {
                in_macro_creation = FALSE;
            }
            else
            {
                /*If the word is not the end of the macro creation, its a macro's content.
                add it to the macro content to be expanded later*/
                strcat(current_macro->content, line_copy);
            }
        }
    }
    /*If there was an error in the macro expansion, return an error*/
    return is_error ? ERROR : SUCCESS;
}

int process_line(MacroNode *head, FILE *output_file, char *word, int *in_macro_creation, char *line_copy, ErrorObject *error)
{
    Macro *current_macro = NULL;
    char *extra_content = NULL; /*Extra content after the macro name / declartion*/

    if (!head || !output_file || !word || !in_macro_creation || !line_copy || !error)
    {
        return ERROR_NULL_PARAM;
    }

    error->code = SUCCESS; /*Reset Error Code*/
    /*If not in macro creation, check if the current word is an existing macro*/
    current_macro = find_macro(head, word);
    if (current_macro != NULL && current_macro->content != NULL)
    {
        /*If the word is a macro, write the macro content to the output file instead of the macro name*/
        extra_content = strtok(NULL, " \n\t");
        if (extra_content != NULL)
        {
            /*If there are extra words after the macro name, return an error and move to the next line*/
            fill_error_object(ERROR_MACRO_EXTRA_CONTENT, 0, word, error);
            return 1;
        }

        /*Write the macro content to the output file instead of the origianl line*/
        fprintf(output_file, "%s", current_macro->content);
    }
    else if (strcmp(word, MACRO_DECLERATION_PREFIX) == 0)
    {
        /* If started macro creating, move word pointer to next word in the line*/
        word = strtok(NULL, " \n\t");
        if (word == NULL)
        {
            /*If no macro name was given, return an error and move to the next line*/
            fill_error_object(ERROR_MACRO_EMPTY_NAME, 0, NULL, error);
            return 1;
        }

        /*Validate the macro name and add it to the macro list*/
        error->code = validate_macro_name(word, head);
        if (error->code != SUCCESS)
        {
            /*If the macro name is invalid, return an error and move to the next line*/
            fill_error_object(error->code, 0, word, error);
            return 1;
        }
        else
        {
            extra_content = strtok(NULL, " \n\t");
            if (extra_content != NULL)
            {
                /*If there are extra words / chars after the macro name*/
                fill_error_object(ERROR_MACRO_DEC_EXTRA_CONTENT, 0, word, error);
                return 1;
            }
            /*Add the macro to the macro list and set the flag to indicate that we are in macro creation*/
            error->code = add_macro(head, word);
            if (error->code != SUCCESS)
            {
                fill_error_object(error->code, 0, word, error);
                return 1;
            }
            *in_macro_creation = TRUE;
        }
    }
    else
    {
        /*If the word is not a macro, and not a macro creation, write the line to the output file as is*/
        fprintf(output_file, "%s", line_copy);
    }
    return SUCCESS;
}