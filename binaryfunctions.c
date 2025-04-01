#include "binaryfunctions.h"

#define OPERATION_LINE_INDEX 0
#define OPERAND1_LINE_INDEX 1
#define OPERAND2_LINE_INDEX 2

BinaryNode *init_binary_image()
{
    BinaryNode *head = (BinaryNode *)malloc(sizeof(BinaryNode));
    if (!head)
    {
        return NULL;
    }
    head->line = NULL;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

int add_binary_line(BinaryLine binaryLine, BinaryNode *head)
{
    BinaryNode *newNode = (BinaryNode *)malloc(sizeof(BinaryNode));
    if (!newNode)
    {
        return ERROR;
    }
    newNode->line = (BinaryLine *)malloc(sizeof(BinaryLine));
    if (!newNode->line)
    {
        free(newNode);
        return ERROR;
    }
    memcpy(newNode->line, &binaryLine, sizeof(BinaryLine));
    newNode->next = head->next; /*Insert at start of list, after head*/
    newNode->prev = head;
    if (head->next != NULL)
    {
        head->next->prev = newNode;
    }

    head->next = newNode;

    return 0;
}

void free_binary_image(BinaryNode *head)
{
    BinaryNode *temp = NULL;

    if (!head)
    {
        return;
    }

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

BinaryLine code_binary(int *IC, char *line, int sourcecode_line_number)
{
    BinaryLine binaryLine = {0, 0, 0, 0};
    Operation *operation = NULL;
    char line_original[MAX_LINE] = {0};
    char *word = NULL;
    OperandType current_oprand_type = NONE;

    strcpy(line_original, line);
    binaryLine.address = *IC;
    binaryLine.num_of_lines = 1;
    binaryLine.sourcecode_line_number = sourcecode_line_number;
    /*binaryLine.code = 0;*/
    binaryLine.code = (unsigned int *)malloc(sizeof(unsigned int) * MAX_OPERANDS + 1);
    if (!binaryLine.code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return binaryLine;
    }
    memset(binaryLine.code, 0, sizeof(unsigned int) * MAX_OPERANDS + 1);

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
            binaryLine.code[OPERATION_LINE_INDEX] |= DIRECT << ADD_MTD_SRC_OFFSET;
            add_empty_line(&binaryLine);
            break;
        case RELATIVE:
            binaryLine.code[OPERATION_LINE_INDEX] |= RELATIVE << ADD_MTD_SRC_OFFSET;
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
            binaryLine.code[OPERATION_LINE_INDEX] |= IMMEDIATE << ADD_MTD_DEST_OFFSET;
            add_line_for_immidiate(word, &binaryLine);
            break;
        case DIRECT:
            binaryLine.code[OPERATION_LINE_INDEX] |= DIRECT << ADD_MTD_DEST_OFFSET;
            add_empty_line(&binaryLine);
            break;
        case RELATIVE:
            binaryLine.code[OPERATION_LINE_INDEX] |= RELATIVE << ADD_MTD_DEST_OFFSET;
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

BinaryLine data_binary(int *DC, char *line, int sourcecode_line_number)
{
    BinaryLine binaryLine = {0, 0, 0, 0};
    char line_original[MAX_LINE] = {0};
    char *word = NULL;

    strcpy(line_original, line);
    binaryLine.address = *DC;
    binaryLine.num_of_lines = 0;
    binaryLine.sourcecode_line_number = sourcecode_line_number;
    /*binaryLine.code = 0;*/

    word = strtok(line, " \t\n");
    if (word == NULL)
    {
        return binaryLine; /*this line will never be reached because we already validated the line*/
    }

    if (strcmp(word, STRING_INSTRUCTION) == 0)
    {
        if (code_string_instruction(word, &binaryLine) == FAILURE)
            return binaryLine;
    }
    else if (strcmp(word, DATA_INSTURCTION) == 0)
    {
        if (code_data_instruction(word, &binaryLine) == FAILURE)
            return binaryLine;
    }

    (*DC) += binaryLine.num_of_lines;
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
    binaryLine->code = (unsigned int *)malloc(sizeof(unsigned int) * strlen(word) + 1);
    if (!binaryLine->code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }
    memset(binaryLine->code, 0, sizeof(unsigned int) * strlen(word) + 1);
    binaryLine->num_of_lines = strlen(word) + 1;
    for (i = 0; i < binaryLine->num_of_lines - 1; i++)
    {
        binaryLine->code[i] = (unsigned int)word[i]; /*Stores ascii value of word[i]*/
    }
    binaryLine->code[binaryLine->num_of_lines - 1] = '\0';
    return SUCCESS;
}

int code_data_instruction(char *word, BinaryLine *binaryLine)
{
    int num_of_integers = 0;
    if (!word || !binaryLine)
    {
        return FAILURE;
    }
    word = strtok(NULL, "");
    trim(word);
    num_of_integers = get_number_of_integers(word);
    if (num_of_integers == ERROR)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }
    binaryLine->code = (unsigned int *)malloc(sizeof(unsigned int) * num_of_integers + 1);
    if (!binaryLine->code)
    {
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return FAILURE;
    }
    memset(binaryLine->code, 0, sizeof(unsigned int) * num_of_integers + 1);
    word = strtok(word, ",");
    while (word != NULL)
    {
        if (binaryLine->num_of_lines <= num_of_integers)
        {
            binaryLine->code[binaryLine->num_of_lines] = atoi(word);
            binaryLine->num_of_lines++;
        }
        word = strtok(NULL, ",");
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
    line_copy = (char *)malloc(strlen(line) + 1);
    if (!line_copy)
    {
        return ERROR;
    }
    strcpy(line_copy, line);
    word = strtok(line_copy, ",");
    while (word != NULL)
    {
        count++;
        word = strtok(NULL, ",");
    }
    free(line_copy);
    return count;
}