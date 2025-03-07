#include "binaryfunctions.h"

BinaryLine code_binary(int *IC, char *line)
{
    BinaryLine binaryLine = {0, 0};
    Operation *operation = NULL;
    char line_original[MAX_LINE] = {0};
    char *word = NULL;
    OperandType current_oprand_type = NONE;

    strcpy(line_original, line);
    binaryLine.address = *IC;
    binaryLine.code = 0;

    word = strtok(line, " \t\n");
    if (word == NULL)
    {
        return binaryLine;
    }
    operation = get_operation(word);
    if (operation == NULL)
    {
        return binaryLine;
    }

    binaryLine.code |= get_code_for_operation(operation);
    binaryLine.code |= 1U << A_OFFSET; /*Set A bit to 1 - Absolute*/

    word = strtok(NULL, ""); /*get rest of line*/
    trim(word);

    if (operation->allowed_source[0] != NONE)
    {
        word = strtok(word, ",");
        current_oprand_type = get_operand_type(word);
        switch (current_oprand_type)
        {
        case REGISTER:
            binaryLine.code |= get_code_for_register(word, TRUE);
            break;
        case IMMEDIATE:
            binaryLine.code |= IMMEDIATE << ADD_MTD_SRC_OFFSET;
            break;
        case DIRECT:
            binaryLine.code |= DIRECT << ADD_MTD_SRC_OFFSET;
            break;
        case RELATIVE:
            binaryLine.code |= RELATIVE << ADD_MTD_SRC_OFFSET;
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
            binaryLine.code |= get_code_for_register(word, FALSE);
            break;
        case IMMEDIATE:
            binaryLine.code |= IMMEDIATE << ADD_MTD_DEST_OFFSET;
            break;
        case DIRECT:
            binaryLine.code |= DIRECT << ADD_MTD_DEST_OFFSET;
            break;
        case RELATIVE:
            binaryLine.code |= RELATIVE << ADD_MTD_DEST_OFFSET;
            break;
        default:
            break;
        }
    }

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
