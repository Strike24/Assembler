#include "preasm.h"

int pre_assembler(char *filename, MacroNode *head)
{
    FILE *input_file, *output_file;

    input_file = open_file(filename, "r", ".as");
    output_file = open_file(filename, "w", ".am");

    if (input_file == NULL || output_file == NULL)
    {
        return ERROR;
    }

    if (macro_expansion(input_file, output_file, head) == ERROR)
    {
        printf("Error: Macro expansion failed\n");
        fclose(input_file);
        fclose(output_file);
        return ERROR;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int macro_expansion(FILE *input_file, FILE *output_file, MacroNode *head)
{
    char line[MAX_LINE];
    char line_copy[MAX_LINE];
    char *word;
    int in_macro_creation = FALSE;
    Macro *current_macro;
    ErrorObject error = {0};
    int line_number = 0;
    /*char macro_content_buffer[MAX_LINE * 100]; = {0};*/

    while (fgets(line, MAX_LINE, input_file))
    {
        line_number++;
        strcpy(line_copy, line);
        word = strtok(line, " \t\n");
        /*
        if (is_label_dec(word, line_number, &error, head))
        {
            word = strtok(NULL, " \t\n");
        }
        */

        if (!word)
        {
            fprintf(output_file, "%s", line_copy);
            continue;
        }

        if (!in_macro_creation)
        {
            current_macro = find_macro(head, word);
            if (current_macro != NULL)
            {
                fprintf(output_file, "%s", current_macro->content);
            }
            else if (strcmp(word, "mcro") == 0)
            {
                /* Move word pointer to next word in the line */
                word = strtok(NULL, " \n\t");
                if (word == NULL)
                {
                    fill_error_object(ERROR_MACRO_EMPTY_NAME, line_number, NULL, &error);
                    handle_error(&error);
                    return ERROR;
                }

                error.code = validate_macro_name(word, head);
                if (error.code != SUCCESS)
                {
                    fill_error_object(error.code, line_number, word, &error);
                    handle_error(&error);
                    return ERROR;
                }
                else
                {
                    add_macro(head, word);
                    in_macro_creation = TRUE;
                }
            }
            else
            {
                fprintf(output_file, "%s", line_copy);
            }
        }
        else
        {
            current_macro = get_current_macro(head);
            if (!current_macro && !current_macro->content)
            {
                fill_error_object(ERROR_MACRO_NOT_DEFINED, line_number, NULL, &error);
                handle_error(&error);
                return ERROR;
            }

            if (strcmp(word, "mcroend") == 0)
            {
                in_macro_creation = FALSE;
            }
            else
            {
                strcat(current_macro->content, line_copy);
            }
        }
    }
    return 0;
}
