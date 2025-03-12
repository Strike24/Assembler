#include "secondpass.h"

int second_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list)
{
    FILE *input_file;
    char line[MAX_LINE];
    int line_number = 1;

    /*Open file after preassmbler macro expantion (.am)*/
    input_file = open_file(filename, "r", POST_MACRO_EXT);
    if (input_file == NULL)
    {
        printf("Error: File not found\n");
        return ERROR;
    }

    while (fgets(line, MAX_LINE, input_file))
    {
        char *word = line;
        Label *entry_label;

        /*If line is a comment, skip it*/
        if (line[0] != ';')
        {
            word = strtok(line, " \t\n");
            if (is_label_dec(word)) /*If label declaration found, skip to next word*/
            {
                word = strtok(NULL, " \t\n");
                word = strtok(NULL, "");
            }

            if (is_entry_operation(word))
            {
                word = strtok(NULL, " \t\n");
                entry_label = find_label(label_list, word);
                if (entry_label == NULL)
                {
                    printf("Error: .entry has an unknown label name \"\n%s\"\n", word);
                    printf("Might not be declared or not exist\n");
                }
                else
                {
                    entry_label->type = ENTRY;
                }
            }
            else if (is_code_operation(word))
            {
                word = strtok(NULL, "");
                fill_missing_label_info(code_image, label_list, word, line_number);
            }
        }

        line_number++;
    }

    build_output_files(filename, code_image, data_image, label_list, 100, 50);
    fclose(input_file);
    return 0;
}

int fill_missing_label_info(BinaryNode *code_image, Label *label_list, char *line, int line_number)
{
    Label *current_label = NULL;
    BinaryLine *current_line = find_by_line_number(code_image, line_number);
    OperandType current_label_type;
    char *word = NULL;
    int i = 0;
    int j = 0;

    if (current_line == NULL)
    {
        return ERROR;
    }
    word = strtok(line, ",");
    trim(word);

    while (word != NULL && i < current_line->num_of_lines)
    {
        current_label_type = get_operand_type(word);
        if (current_label_type != RELATIVE && current_label_type != DIRECT)
        {
            word = strtok(NULL, ",");
            i++;
            continue;
        }

        if (current_label_type == RELATIVE)
        {
            word++; /*Skip the &*/
        }

        current_label = find_label(label_list, word);
        if (current_label == NULL)
        {
            printf("Error: Unknown label name \"%s\"\n", word);
            return ERROR;
        }

        if (current_label_type == RELATIVE)
        {
            for (j = 0; j < current_line->num_of_lines; j++)
            {
                if (current_line->code[j] == 0)
                {
                    current_line->code[j] = current_label->address - current_line->address;
                    break;
                }
                else if (i == current_line->num_of_lines - 1)
                {
                    printf("Error: Label address already filled\n");
                    return ERROR;
                }
            }
        }
        else if (current_label_type == DIRECT)
        {
            for (j = 0; j < current_line->num_of_lines; j++)
            {
                if (current_line->code[j] == 0)
                {
                    if (current_label->type == EXTERNAL)
                    {
                        current_line->code[j] |= 1U << E_OFFSET;
                    }
                    else
                    {
                        current_line->code[j] |= 1U << R_OFFSET;
                    }

                    current_line->code[j] |= (current_label->address) << ARE_OFFSET;
                    break;
                }
                else if (i == current_line->num_of_lines - 1)
                {
                    printf("Error: Label address already filled\n");
                    return ERROR;
                }
            }
        }

        i++;
        word = strtok(NULL, " \t\n");
    }

    return 0;
}

int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, int ICF, int DCF)
{
    FILE *ob_file;
    FILE *ent_file;
    FILE *ext_file;
    BinaryNode *current_node = code_image;
    Label *current_label = label_list;
    int is_ent_file = FALSE;
    int is_ext_file = FALSE;

    if (code_image->next != NULL || data_image->next != NULL)
    {
        ob_file = open_file(filename, "w", OB_EXT);
        if (ob_file == NULL)
        {
            printf("Error: Failed to create .ob file\n");
            return ERROR;
        }
    }

    while (current_label != NULL)
    {
        if (current_label->type == ENTRY)
        {
            ent_file = open_file(filename, "w", ENT_EXT);
            if (ent_file == NULL)
            {
                printf("Error: Failed to create .ent file\n");
                return ERROR;
            }
            is_ent_file = TRUE;
        }
        else if (current_label->type == EXTERNAL)
        {
            ext_file = open_file(filename, "w", EXT_EXT);
            if (ext_file == NULL)
            {
                printf("Error: Failed to create .ext file\n");
                return ERROR;
            }
            is_ext_file = TRUE;
        }
        current_label = current_label->next;
    }

    fprintf(ob_file, "%7d %-6d\n", ICF, DCF);
    current_node = code_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != code_image)
    {
        print_line_ob(ob_file, current_node->line);
        current_node = current_node->prev;
    }

    current_node = data_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != data_image)
    {
        print_line_ob(ob_file, current_node->line);
        current_node = current_node->prev;
    }
    fclose(ob_file);

    current_label = label_list;
    if (is_ent_file)
    {

        while (current_label != NULL)
        {
            if (current_label->type == ENTRY)
            {
                fprintf(ent_file, "%s %07d\n", current_label->name, current_label->address);
            }
            current_label = current_label->next;
        }
        fclose(ent_file);
    }

    current_label = label_list;
    if (is_ext_file)
    {
        while (current_label != NULL)
        {
            if (current_label->type == EXTERNAL)
            {
                fprintf(ext_file, "%s %07d\n", current_label->name, current_label->address);
            }
            current_label = current_label->next;
        }
        fclose(ext_file);
    }

    return 0;
}

int print_line_ob(FILE *ob_file, BinaryLine *current_node)
{
    int i = 0;
    if (current_node == NULL)
    {
        return ERROR;
    }
    for (i = 0; i < current_node->num_of_lines; i++)
    {
        fprintf(ob_file, "%07d %04X\n", current_node->address + i, current_node->code[i]);
    }
    return 0;
}