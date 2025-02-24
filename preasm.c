#include "preasm.h"

int pre_assembler(char *filename)
{
    FILE *input_file, *output_file;
    add_extension(filename, ".as");
    input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        printf("Error: File not found\n");
        return 1;
    }

    replace_extension(filename, ".am");
    output_file = fopen(filename, "w");
    if (output_file == NULL)
    {
        printf("Error: File not found\n");
        fclose(input_file);
        return 1;
    }

    if (macro_expansion(input_file, output_file) == 1)
    {
        printf("Error: Macro expansion failed\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

int macro_expansion(FILE *input_file, FILE *output_file)
{
    MacroNode *head = init_macro_table();
    Macro *temp;
    char line[MAX_LINE];
    char line_copy[MAX_LINE];
    char *word;
    int in_macro_creation = 0;
    Macro *current_macro;
    char *temp_name;

    while (fgets(line, MAX_LINE, input_file))
    {
        printf("%s", line);
        strcpy(line_copy, line);
        word = strtok(line, " \t\n");

        if (!word)
        {
            fprintf(output_file, "%s", line_copy);
            continue;
        }

        if (!in_macro_creation)
        {
            if ((temp = find_macro(head, word)) != NULL)
            {
                fprintf(output_file, "%s", temp->content);
            }
            else if (strcmp(word, "mcro") == 0)
            {
                in_macro_creation = 1;
                temp_name = strtok(NULL, " \n\t");
                if (is_valid_macro_name(temp_name) == 0)
                {
                    printf("Error: Macro name missing / invalid.\nMacro names can't be type of a label.\n");
                    return 1;
                }

                add_macro(head, temp_name);
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
                return 1;
            }

            if (strcmp(word, "mcroend") == 0)
            {
                in_macro_creation = 0;
            }
            else
            {
                strcat(current_macro->content, line_copy);
            }
        }
    }
    print_macro_table(head);
    free_macro_table(head);
    return 0;
}
