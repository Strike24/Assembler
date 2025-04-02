#include "binaryfunctions.h"

#define OPERATION_LINE_INDEX 0
#define OPERAND1_LINE_INDEX 1
#define OPERAND2_LINE_INDEX 2

BinaryNode *init_binary_image()
{
    BinaryNode *head = (BinaryNode *)malloc(sizeof(BinaryNode)); /*Allocates size for binary node*/
    if (!head)
    {
        return NULL;
    }
    head->line = NULL;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

int add_binary_line(BinaryLine *binaryLine, BinaryNode *head)
{
    BinaryNode *newNode = (BinaryNode *)malloc(sizeof(BinaryNode)); /*Allocates size for binary node*/
    if (!newNode)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return ERROR;
    }
    if (!binaryLine || !head)
    {
        free(newNode);
        return ERROR;
    }
    newNode->line = binaryLine; /*Set line of new node to binary line*/

    newNode->next = head->next; /*Insert at start of list, after head*/
    newNode->prev = head;
    if (head->next != NULL)
    {
        head->next->prev = newNode; /*Set prev of next node to new node*/
    }

    head->next = newNode; /*Set next of head to new node*/

    return 0;
}

void free_binary_image(BinaryNode *head)
{
    BinaryNode *temp = NULL;

    if (!head)
    {
        return;
    }

    /*Free the list by looping over it and freeing every BinaryNode*/
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        if (temp->line)
        {
            if (temp->line->code)
            {
                free(temp->line->code);
            }
            free(temp->line);
        }
        free(temp);
    }
}

BinaryLine *find_by_line_number(BinaryNode *head, int line_number)
{
    BinaryNode *current = head->next;
    /*Search BianryNode by source code line number and return pointer*/
    while (current != NULL)
    {
        if (current->line->sourcecode_line_number == line_number)
        {
            return current->line;
        }
        current = current->next;
    }
    return NULL;
}

BinaryLine *code_binary(int *IC, char *line, int sourcecode_line_number)
{
    Operation *operation = NULL;
    char *word = NULL;                                                 /*Word to hold the current word in the line*/
    OperandType current_oprand_type = NONE;                            /*Current operand type*/
    BinaryLine *binaryLine = (BinaryLine *)malloc(sizeof(BinaryLine)); /*Allocates size for binary line*/

    if (!binaryLine)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    if (!line || !IC)
    {
        return NULL;
    }

    binaryLine->address = *IC;
    binaryLine->num_of_lines = 1;
    binaryLine->sourcecode_line_number = sourcecode_line_number;

    binaryLine->code = (unsigned int *)malloc(sizeof(unsigned int) * MAX_OPERANDS + 1); /*Allocates size for code array*/
    if (!binaryLine->code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }
    memset(binaryLine->code, 0, sizeof(unsigned int) * MAX_OPERANDS + 1); /*Set all code to 0 to avoid garbage values*/

    word = strtok(line, " \t\n"); /*Get first word in line*/
    if (word == NULL)
    {
        return NULL; /*this line will never be reached because we already validated the line*/
    }
    operation = get_operation(word);
    if (operation == NULL)
    {
        return NULL; /*this line will never be reached because we already validated the line*/
    }

    binaryLine->code[OPERATION_LINE_INDEX] |= get_code_for_operation(operation); /*Get code for operation (opcode and funct), and set it in the first line*/
    binaryLine->code[OPERATION_LINE_INDEX] |= 1U << A_OFFSET;                    /*Set A bit to 1 - Absolute*/

    word = strtok(NULL, ""); /*get rest of line*/
    trim(word);

    if (operation->allowed_source[0] != NONE) /*If there is a source operand*/
    {
        word = strtok(word, ",");
        current_oprand_type = get_operand_type(word); /*get operand type*/
        switch (current_oprand_type)
        {
        case REGISTER:
            /*If operand is a register, get code for register and set it in the first line*/
            binaryLine->code[OPERATION_LINE_INDEX] |= get_code_for_register(word, TRUE);
            break;
        case IMMEDIATE:
            /*If operand is an immediate, set the immediate bit and add empty line for immediate with the number code*/
            binaryLine->code[OPERATION_LINE_INDEX] |= IMMEDIATE << ADD_MTD_SRC_OFFSET;
            add_line_for_immidiate(word, binaryLine);
            break;
        case DIRECT:
            /*If operand is a direct, set the direct bit and add empty line (filled in the second pass)*/
            binaryLine->code[OPERATION_LINE_INDEX] |= DIRECT << ADD_MTD_SRC_OFFSET;
            add_empty_line(binaryLine);
            break;
        case RELATIVE:
            /*If operand is a relative, set the relative bit and add empty line (filled in the second pass)*/
            binaryLine->code[OPERATION_LINE_INDEX] |= RELATIVE << ADD_MTD_SRC_OFFSET;
            add_empty_line(binaryLine);
            break;
        default:
            break;
        }
        word = strtok(NULL, ","); /*Get next operand (target)*/
    }

    if (operation->allowed_target[0] != NONE) /*if there is a target operand*/
    {
        current_oprand_type = get_operand_type(word); /*get operand type*/
        switch (current_oprand_type)
        {
        case REGISTER:
            /*If operand is a register, get code for register and set it in the first line*/
            binaryLine->code[OPERATION_LINE_INDEX] |= get_code_for_register(word, FALSE);
            break;
        case IMMEDIATE:
            /*If operand is an immediate, set the immediate bit and add empty line for immediate with the number code*/
            binaryLine->code[OPERATION_LINE_INDEX] |= IMMEDIATE << ADD_MTD_DEST_OFFSET;
            add_line_for_immidiate(word, binaryLine);
            break;
        case DIRECT:
            /*If operand is a direct, set the direct bit and add empty line (filled in the second pass)*/
            binaryLine->code[OPERATION_LINE_INDEX] |= DIRECT << ADD_MTD_DEST_OFFSET;
            add_empty_line(binaryLine);
            break;
        case RELATIVE:
            /*If operand is a relative, set the relative bit and add empty line (filled in the second pass)*/
            binaryLine->code[OPERATION_LINE_INDEX] |= RELATIVE << ADD_MTD_DEST_OFFSET;
            add_empty_line(binaryLine);
            break;
        default:
            break;
        }
    }

    (*IC) += binaryLine->num_of_lines; /*Increment IC by number of lines added*/
    return binaryLine;
}

unsigned int get_code_for_operation(Operation *pOperation)
{
    unsigned result = 0;
    int temp = pOperation->opcode << OPCODE_OFFSET; /*Shift opcode to the right place in memory*/
    result |= temp;
    temp = pOperation->funct << FUNCT_OFFSET; /*Shift funct to the right place in memory*/
    result |= temp;
    return result; /*Return the code for the first line with operation opcode and funct*/
}

unsigned int get_code_for_register(char *register_name, int is_source)
{
    unsigned int result = 0;

    int register_index = get_register_index(register_name); /*Get register index 0-7*/

    if (register_index == -1)
    {
        return 0;
    }

    if (is_source) /*If source operand*/
    {
        result |= REGISTER << ADD_MTD_SRC_OFFSET;   /*Set source method to register*/
        result |= register_index << SRC_REG_OFFSET; /*Set register index in the right place*/
    }
    else
    {
        result |= REGISTER << ADD_MTD_DEST_OFFSET;   /*Set destination method to register*/
        result |= register_index << DEST_REG_OFFSET; /*Set register index in the right place*/
    }
    return result;
}

void add_line_for_immidiate(char *operand, BinaryLine *binaryLine)
{
    int num = 0;
    if (!operand || !binaryLine)
    {
        return;
    }
    /*Convert operand to number*/
    num = atoi(operand + 1); /*Avoid # at start of immidiate operand number*/
    if (binaryLine->num_of_lines <= MAX_OPERANDS)
    {
        binaryLine->code[binaryLine->num_of_lines] |= num << ARE_OFFSET;
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

BinaryLine *data_binary(int *DC, char *line, int sourcecode_line_number)
{
    BinaryLine *binaryLine = (BinaryLine *)malloc(sizeof(BinaryLine)); /*Allocates size for binary line*/
    char line_original[MAX_LINE] = {0};
    char *word = NULL;

    if (!binaryLine)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }

    if (!line || !DC)
    {
        free(binaryLine);
        return NULL;
    }

    strcpy(line_original, line);
    binaryLine->address = *DC; /*Set address to DC*/
    binaryLine->num_of_lines = 0;
    binaryLine->sourcecode_line_number = sourcecode_line_number;

    word = strtok(line, " \t\n");
    if (word == NULL)
    {
        return NULL; /*this line will never be reached because we already validated the line*/
    }

    if (strcmp(word, STRING_INSTRUCTION) == 0)
    {
        /*If line is a string instruction, code string*/
        if (code_string_instruction(word, binaryLine) == FAILURE)
            return NULL;
    }
    else if (strcmp(word, DATA_INSTRUCTION) == 0)
    {
        /*If line is a data instruction, code data*/
        if (code_data_instruction(word, binaryLine) == FAILURE)
            return NULL;
    }

    (*DC) += binaryLine->num_of_lines; /*Increment DC by number of lines added*/
    return binaryLine;
}

int code_string_instruction(char *word, BinaryLine *binaryLine)
{
    int i = 0;
    if (!word || !binaryLine)
    {
        return FAILURE;
    }
    word = strtok(NULL, "");
    trim(word);
    word++;                        /*Skip the first '"'  */
    word[strlen(word) - 1] = '\0'; /*remove the last '"' */

    binaryLine->code = (unsigned int *)malloc(sizeof(unsigned int) * strlen(word) + 1); /*Allocates size for code array*/
    if (!binaryLine->code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }
    memset(binaryLine->code, 0, sizeof(unsigned int) * strlen(word) + 1); /*Set all code to 0 to avoid garbage values*/

    /*Set the string in the code array*/
    binaryLine->num_of_lines = strlen(word) + 1; /*+1 for the null terminator*/
    for (i = 0; i < binaryLine->num_of_lines - 1; i++)
    {
        binaryLine->code[i] = (unsigned int)word[i]; /*Stores ascii value of word[i]*/
    }
    binaryLine->code[binaryLine->num_of_lines - 1] = '\0'; /*Set null terminator*/
    return SUCCESS;
}

int code_data_instruction(char *word, BinaryLine *binaryLine)
{
    int num_of_integers = 0; /*Number of integers in the line*/
    if (!word || !binaryLine)
    {
        return FAILURE;
    }

    word = strtok(NULL, ""); /*Get rest of line*/
    trim(word);

    num_of_integers = get_number_of_integers(word); /*Get number of integers in the line*/
    if (num_of_integers == ERROR)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }

    binaryLine->code = (unsigned int *)malloc(sizeof(unsigned int) * num_of_integers + 1); /*Allocates size for code array based on number of integers*/
    if (!binaryLine->code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }
    memset(binaryLine->code, 0, sizeof(unsigned int) * num_of_integers + 1); /*Set all code to 0 to avoid garbage values*/

    word = strtok(word, ","); /*Get first integer*/
    while (word != NULL)
    {
        if (binaryLine->num_of_lines <= num_of_integers)
        {
            binaryLine->code[binaryLine->num_of_lines] = atoi(word); /*Convert string to integer and store it in code array*/
            binaryLine->num_of_lines++;
        }
        word = strtok(NULL, ","); /*Get next integer*/
    }
    return SUCCESS;
}

int get_number_of_integers(char *line)
{
    char *line_copy = NULL;
    int count = 0;
    char *word = NULL;
    if (!line)
        return ERROR;

    line_copy = (char *)malloc(strlen(line) + 1); /*Allocates size for line copy*/
    if (!line_copy)
    {
        return ERROR;
    }

    strcpy(line_copy, line); /*Copy line to new string to avoid strtok changing the original*/

    word = strtok(line_copy, ","); /*Get first integer*/
    while (word != NULL)           /*Count integers in line*/
    {
        count++;
        word = strtok(NULL, ",");
    }
    free(line_copy);
    return count;
}