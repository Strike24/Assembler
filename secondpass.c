#include "secondpass.h"

int second_pass(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, int ICF, int DCF, int is_error)
{
    FILE *input_file;
    char line[MAX_LINE];
    int line_number = 1;
    ExternLabel *extern_label_list = init_extern_label_table();
    Label *current_label = NULL;
    char *word = NULL;
    ErrorObject error = {0};

    /*Open file after preassmbler macro expantion (.am)*/
    input_file = open_file(filename, "r", POST_MACRO_EXT);
    if (input_file == NULL)
    {
        printf("Error: File not found\n");
        return ERROR;
    }

    while (fgets(line, MAX_LINE, input_file))
    {
        word = line;
        error.code = SUCCESS;

        /*If line is a comment, skip it*/
        if (line[0] != ';')
        {
            word = strtok(line, " \t\n");
            /*Null in macro_list because its use is not needed here*/
            if (is_label_dec(word, line_number, NULL, NULL)) /*If label declaration found, skip to next word*/
            {
                word = strtok(NULL, " \t\n");
                /*word = strtok(NULL, "");*/
            }

            if (is_entry_operation(word))
            {
                word = strtok(NULL, " \t\n");
                current_label = find_label(label_list, word);
                if (current_label == NULL)
                {
                    fill_error_object(ERROR_LABEL_NOT_DEFINED, line_number, word, &error);
                    handle_error(&error);
                    is_error = TRUE;
                }
                else if (current_label->type == EXTERNAL)
                {
                    fill_error_object(ERROR_LABEL_IS_EXTERNAL, line_number, word, &error);
                    handle_error(&error);
                    is_error = TRUE;
                }
                else
                {
                    current_label->type = ENTRY;
                }
            }
            else if (is_code_operation(word))
            {
                word = strtok(NULL, "");

                fill_missing_label_info(code_image, label_list, word, line_number, extern_label_list, &error);
                if (error.code != SUCCESS)
                {

                    handle_error(&error);
                    is_error = TRUE;
                }
            }
        }

        line_number++;
    }

    align_memory_to_bits(code_image, data_image);
    if (is_error)
    {
        printf("\nErrors found in the assembly process, output files not built.\n");
    }
    else
    {
        build_output_files(filename, code_image, data_image, label_list, extern_label_list, ICF, DCF);
    }
    fclose(input_file);
    free_extern_label_table(extern_label_list);
    return 0;
}

int fill_missing_label_info(BinaryNode *code_image, Label *label_list, char *line, int line_number, ExternLabel *extern_list, ErrorObject *error)
{
    Label *current_label = NULL;
    BinaryLine *current_line = find_by_line_number(code_image, line_number);
    OperandType current_operand_type;
    char *word = NULL;
    int i = 0;
    int j = 0;

    if (current_line == NULL)
    {
        return FAILURE; /*Line not found*/
    }
    trim(line);
    word = strtok(line, ",");
    /*trim(word);*/

    while (word != NULL && i < current_line->num_of_lines)
    {
        current_operand_type = get_operand_type(word);
        if (current_operand_type != RELATIVE && current_operand_type != DIRECT)
        {
            word = strtok(NULL, ",");
            i++;
            continue;
        }

        if (current_operand_type == RELATIVE)
        {
            word++; /*Skip the &*/
        }

        current_label = find_label(label_list, word);
        if (current_label == NULL)
        {
            fill_error_object(ERROR_LABEL_NOT_DEFINED, line_number, word, error);
            return FAILURE;
        }

        if (current_label->type == EXTERNAL)
        {
            /*Add 1 to the address because address of usage is needed and not order of oprand*/
            add_extern_label(extern_list, current_label, current_line->address + i + 1);
        }

        if (current_operand_type == RELATIVE)
        {
            for (j = 0; j < current_line->num_of_lines; j++)
            {
                if (current_line->code[j] == 0)
                {
                    current_line->code[j] = (current_label->address - current_line->address) << ARE_OFFSET;
                    current_line->code[j] |= 1U << A_OFFSET;
                    break;
                }
                else if (i == current_line->num_of_lines - 1)
                {
                    printf("Error: Label address already filled\n");
                    return FAILURE;
                }
            }
        }
        else if (current_operand_type == DIRECT)
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
                /*  else if (i == current_line->num_of_lines - 1)
                  {
                      printf("Error: Label address already filled\n");
                      return ERROR;
                  }*/
            }
        }

        i++;
        word = strtok(NULL, " \t\n");
    }

    return SUCCESS;
}

int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, ExternLabel *extern_list, int ICF, int DCF)
{
    FILE *ob_file;
    FILE *ent_file;
    FILE *ext_file;
    BinaryNode *current_node = code_image;
    Label *current_label = label_list;
    ExternLabel *current_extern_label = extern_list;
    int is_ent_file = FALSE;
    int is_ext_file = FALSE;

    if (code_image->next != NULL || data_image->next != NULL)
    {
        ob_file = open_file(filename, "w", OB_EXT);
        if (ob_file == NULL)
        {

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
                return ERROR;
            }
            is_ent_file = TRUE;
        }
        else if (current_label->type == EXTERNAL)
        {
            ext_file = open_file(filename, "w", EXT_EXT);
            if (ext_file == NULL)
            {
                return ERROR;
            }
            is_ext_file = TRUE;
        }
        current_label = current_label->next;
    }

    fprintf(ob_file, "%7d %-6d\n", ICF - 100, DCF);
    current_node = code_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != code_image)
    {
        print_line_ob(ob_file, current_node->line, 0);
        current_node = current_node->prev;
    }

    current_node = data_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != data_image)
    {
        print_line_ob(ob_file, current_node->line, ICF);
        current_node = current_node->prev;
    }
    fclose(ob_file);

    current_label = label_list;
    if (is_ent_file && current_label != NULL)
    {
        while (current_label->next != NULL)
        {
            current_label = current_label->next;
        }
        while (current_label != label_list)
        {

            if (current_label->type == ENTRY)
            {
                fprintf(ent_file, "%s %07d\n", current_label->name, current_label->address);
            }
            current_label = current_label->prev;
        }
        fclose(ent_file);
    }

    current_extern_label = extern_list;
    if (is_ext_file)
    {
        while (current_extern_label->next != NULL)
        {
            current_extern_label = current_extern_label->next;
        }
        while (current_extern_label != extern_list)
        {
            if (current_extern_label->usage_address != 0 && current_extern_label->label->name != NULL)
            {
                fprintf(ext_file, "%s %07d\n", current_extern_label->label->name, current_extern_label->usage_address);
            }
            current_extern_label = current_extern_label->prev;
        }
        fclose(ext_file);
    }

    printf("* No errors found in the assembly process, output files built. *\n");
    return 0;
}

int print_line_ob(FILE *ob_file, BinaryLine *current_node, int starting_index)
{
    int i = 0;
    if (current_node == NULL)
    {
        return ERROR;
    }
    for (i = 0; i < current_node->num_of_lines; i++)
    {
        fprintf(ob_file, "%07d %06x\n", current_node->address + i + starting_index, current_node->code[i]);
    }
    return 0;
}

int align_memory_to_bits(BinaryNode *code_image, BinaryNode *data_image)
{
    BinaryNode *current_node = code_image;
    BinaryLine *current_line;
    int i = 0;

    if (!code_image || !data_image)
    {
        return ERROR;
    }
    while (current_node->next != NULL)
    {
        if (current_node->line == NULL)
        {
            current_node = current_node->next;
            continue;
        }
        current_line = current_node->line;
        for (i = 0; i < current_line->num_of_lines; i++)
        {
            current_line->code[i] &= BITS_MASK;
        }
        current_node = current_node->next;
    }

    current_node = data_image;
    while (current_node->next != NULL)
    {
        if (current_node->line == NULL)
        {
            current_node = current_node->next;
            continue;
        }
        current_line = current_node->line;
        for (i = 0; i < current_line->num_of_lines; i++)
        {
            current_line->code[i] &= BITS_MASK;
        }
        current_node = current_node->next;
    }
    return 0;
}
