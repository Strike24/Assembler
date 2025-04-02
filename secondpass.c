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
                /*If line is an entry operation, find the label and set it as entry*/
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
                    /*If label is external it can't be entry*/
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
                /*If line is a code operation, check if there are any direct or relative operands needing to be filled*/
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

    /*Align memory to 24 bits and cut off the rest*/
    align_memory_to_bits(code_image, data_image);

    /*If errors were found, do not build the output files.*/
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

    /*loop over all operands in the line*/
    while (word != NULL && i < current_line->num_of_lines)
    {
        current_operand_type = get_operand_type(word);
        if (current_operand_type != RELATIVE && current_operand_type != DIRECT)
        {
            /*If operand is not relative or direct, skip to next one*/
            word = strtok(NULL, ",");
            i++;
            continue;
        }

        if (current_operand_type == RELATIVE)
        {
            word++; /*Skip the &*/
        }

        /*Check if the label is defined*/
        current_label = find_label(label_list, word);
        if (current_label == NULL)
        {
            fill_error_object(ERROR_LABEL_NOT_DEFINED, line_number, word, error);
            return FAILURE;
        }

        /*If label is external, add it to the extern usage list*/
        if (current_label->type == EXTERNAL)
        {
            /*Add 1 to the address because address of usage is needed and not order of oprand*/
            add_extern_label(extern_list, current_label, current_line->address + i + 1);
        }

        if (current_operand_type == RELATIVE)
        {
            /*Loop over all lines of binary code in the current address*/
            for (j = 0; j < current_line->num_of_lines; j++)
            {
                if (current_line->code[j] == 0)
                {
                    /*If operand is relative, set the code to the label address relative to the current line address*/
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
            /*Loop over all lines of binary code in the current address*/
            for (j = 0; j < current_line->num_of_lines; j++)
            {
                if (current_line->code[j] == 0)
                {
                    if (current_label->type == EXTERNAL)
                    {
                        /*If label is external, set the E bit to 1*/
                        current_line->code[j] |= 1U << E_OFFSET;
                    }
                    else
                    {
                        /*If label is not external its relocatable, set the R bit to 1*/
                        current_line->code[j] |= 1U << R_OFFSET;
                    }

                    /*Set the code to the label address*/
                    current_line->code[j] |= (current_label->address) << ARE_OFFSET;
                    break;
                }
            }
        }

        i++;
        word = strtok(NULL, " \t\n"); /*Get next operand*/
    }

    return SUCCESS;
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

    /*Loops over every binary line in the code image*/
    while (current_node->next != NULL)
    {
        if (current_node->line == NULL)
        {
            /*If line is NULL, skip to next one*/
            current_node = current_node->next;
            continue;
        }

        current_line = current_node->line;
        /*Loops over every line of binary code in the current address and masks it.
        This will turn of every bit after the 23rd bit*/
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
            /*If line is NULL, skip to next one*/
            current_node = current_node->next;
            continue;
        }
        current_line = current_node->line;
        for (i = 0; i < current_line->num_of_lines; i++)
        {
            /*Loops over every line of binary code in the current address and masks it.*/
            current_line->code[i] &= BITS_MASK;
        }
        current_node = current_node->next;
    }
    return 0;
}
