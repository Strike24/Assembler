#include "binaryfunctions.h"

#define OPERATION_LINE_INDEX 0
#define OPERAND1_LINE_INDEX 1
#define OPERAND2_LINE_INDEX 2

BinaryLine code_binary(int *IC, char *line)
{
    BinaryLine binaryLine = {0, {0}, 0};
    Operation *operation = NULL;
    char line_original[MAX_LINE] = {0};
    char *word = NULL;
    OperandType current_oprand_type = NONE;

    strcpy(line_original, line);
    binaryLine.address = *IC;
    binaryLine.num_of_lines = 1;
    /*binaryLine.code = 0;*/

    word = strtok(line, " \t\n");
    if (word == NULL)
    {
        return binaryLine;
    }
    operation = get_operation(word);
    if (operation == NULL)
    {
        printf("Error: Invalid operation %s\n", word);
        return binaryLine;
    }

    binaryLine.code[OPERATION_LINE_INDEX] |= get_code_for_operation(operation);
    binaryLine.code[OPERATION_LINE_INDEX] |= 1U << A_OFFSET; /*Set A bit to 1 - Absolute*/

    word = strtok(NULL, ""); /*get rest of line*/
    trim(word);

    if (operation->allowed_source[0] != NONE)
    {
        word = strtok(word, ",");
        current_oprand_type = get_operand_type(word);
        switch (current_oprand_type)
        {
        case REGISTER:
            binaryLine.code[OPERATION_LINE_INDEX] |= get_code_for_register(word, TRUE);
            break;
        case IMMEDIATE:
            binaryLine.code[OPERATION_LINE_INDEX] |= IMMEDIATE << ADD_MTD_SRC_OFFSET;
            add_line_for_immidiate(word, &binaryLine);
            break;
        case DIRECT:
            binaryLine.code[binaryLine.num_of_lines - 1] |= DIRECT << ADD_MTD_DEST_OFFSET;
            add_empty_line(&binaryLine);
            break;
        case RELATIVE:
            binaryLine.code[binaryLine.num_of_lines - 1] |= RELATIVE << ADD_MTD_DEST_OFFSET;
            add_empty_line(&binaryLine);
            break;
        default:
            break;
        }
        word = strtok(NULL, ",");
    }

    if (operation->allowed_target[0] != NONE)
    {
        current_oprand_type = get_operand_type(word);
        switch (current_oprand_type)
        {
        case REGISTER:
            binaryLine.code[OPERATION_LINE_INDEX] |= get_code_for_register(word, FALSE);
            break;
        case IMMEDIATE:
            binaryLine.code[OPERATION_LINE_INDEX] |= IMMEDIATE << ADD_MTD_SRC_OFFSET;
            add_line_for_immidiate(word, &binaryLine);
            break;
        case DIRECT:
            binaryLine.code[binaryLine.num_of_lines - 1] |= DIRECT << ADD_MTD_DEST_OFFSET;
            add_empty_line(&binaryLine);
            break;
        case RELATIVE:
            binaryLine.code[binaryLine.num_of_lines - 1] |= RELATIVE << ADD_MTD_DEST_OFFSET;
            add_empty_line(&binaryLine);
            break;
        default:
            break;
        }
    }

    (*IC) += binaryLine.num_of_lines;
    return binaryLine;
}

unsigned int get_code_for_operation(Operation *pOperation)
{
    unsigned result = 0;
    int temp = pOperation->opcode << OPCODE_OFFSET;
    result |= temp;
    temp = pOperation->funct << FUNCT_OFFSET;
    result |= temp;
    return result;
}

unsigned int get_code_for_register(char *register_name, int is_source)
{
    unsigned int result = 0;

    int register_index = get_register_index(register_name);

    if (register_index == -1)
    {
        return 0;
    }

    if (is_source)
    {
        result |= REGISTER << ADD_MTD_SRC_OFFSET;
        result |= register_index << SRC_REG_OFFSET;
    }
    else
    {
        result |= REGISTER << ADD_MTD_DEST_OFFSET;
        result |= register_index << DEST_REG_OFFSET;
    }
    return result;
}

void add_line_for_immidiate(char *operand, BinaryLine *binaryLine)
{
    int num = atoi(operand + 1); /*Avoid # at start of immidiate operand number*/
    if (binaryLine->num_of_lines <= MAX_OPERANDS)
    {
        binaryLine->code[binaryLine->num_of_lines] |= num << IMMEDIATE_OFFSET;
        binaryLine->code[binaryLine->num_of_lines] |= 1U << A_OFFSET; /*Set A bit to 1 - Absolute*/
        binaryLine->num_of_lines++;                                   /*Increment number of lines*/
    }
}

void add_empty_line(BinaryLine *binaryLine)
{
    if (binaryLine->num_of_lines <= MAX_OPERANDS)
    {
        binaryLine->code[binaryLine->num_of_lines] = 0;
        binaryLine->num_of_lines++; /*Increment number of lines*/
    }
}
