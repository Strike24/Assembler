#include "operations.h"

Operation OPERATIONS[NUM_OF_OPERATIONS] = {
    {"mov", 0, 0, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"cmp", 1, 0, {IMMEDIATE, DIRECT, REGISTER}, {IMMEDIATE, DIRECT, REGISTER}},
    {"add", 2, 1, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"sub", 2, 2, {IMMEDIATE, DIRECT, REGISTER}, {DIRECT, REGISTER}},
    {"lea", 4, 0, {DIRECT, IMMEDIATE}, {DIRECT, REGISTER}},
    {"clr", 5, 1, {0}, {DIRECT, REGISTER}},
    {"not", 5, 2, {0}, {DIRECT, REGISTER}},
    {"inc", 5, 3, {0}, {DIRECT, REGISTER}},
    {"dec", 5, 4, {0}, {DIRECT, REGISTER}},
    {"jmp", 9, 1, {0}, {DIRECT, RELATIVE}},
    {"bne", 9, 2, {0}, {DIRECT, RELATIVE}},
    {"jsr", 9, 3, {0}, {DIRECT, RELATIVE}},
    {"red", 12, 0, {0}, {DIRECT, REGISTER}},
    {"prn", 13, 0, {0}, {IMMEDIATE, DIRECT, REGISTER}},
    {"rts", 14, 0, {0}, {0}},
    {"stop", 15, 0, {0}, {0}}};

char *register_names[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int is_operation_name(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int is_register_name(char *name)
{
    int i;
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
        if (strcmp(register_names[i], name) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

OperandType *get_allowed_addressing_methods(char *name, int source_or_target)
{
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++)
    {
        if (strcmp(OPERATIONS[i].name, name) == 0)
        {
            if (source_or_target == SOURCE)
            {
                return OPERATIONS[i].allowed_source;
            }
            else if (source_or_target == TARGET)
            {
                return OPERATIONS[i].allowed_target;
            }
        }
    }
    return NULL;
}