#include "preasm.h"

int pre_assembler(char *filename)
{
    FILE *input_file, *output_file;

    input_file = open_file(filename, "r", ".as");
    output_file = open_file(filename, "w", ".am");

    if (input_file == NULL || output_file == NULL)
    {
        return ERROR;
    }

    if (macro_expansion(input_file, output_file) == ERROR)
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

int macro_expansion(FILE *input_file, FILE *output_file)
{
    MacroNode *head = init_macro_table();
    char line[MAX_LINE];
    char line_copy[MAX_LINE];
    char *word;
    int in_macro_creation = 0;
    Macro *current_macro;
    /*char macro_content_buffer[MAX_LINE * 100]; = {0};*/

    while (fgets(line, MAX_LINE, input_file))
    {
        strcpy(line_copy, line);
        word = strtok(line, " \t\n");
        if (is_label_dec(word, -1)) /*If label is found, turn on flag, skip to next word*/
        {
            word = strtok(NULL, " \t\n");
        }

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
                in_macro_creation = TRUE;
                /* Move word pointer to next word in the line */
                word = strtok(NULL, " \n\t");
                if (word == NULL)
                {
                    printf("Error: Macro name missing / invalid.\nMacro names can't be type of a label.\n");
                    return ERROR;
                }

                add_macro(head, word);
            }
            else
            {
                fprintf(output_file, "%s", line_copy);
            }
        }
        else
        {
            current_macro = get_current_macro(head);
            if (!current_macro)
            {
                printf("Error: No macro found while adding content.\n");
                return ERROR;
            }

            if (strcmp(word, "mcroend") == 0)
            {
                in_macro_creation = FALSE;
                /*
                char *temp = (char *)malloc(strlen(current_macro->content) + 1);
                strcpy(temp, current_macro->content);
                free(current_macro->content);
                current_macro->content = temp;
                */
            }
            else
            {
                strcat(current_macro->content, line_copy);
            }
        }
    }
    free_macro_table(head);
    return 0;
}
