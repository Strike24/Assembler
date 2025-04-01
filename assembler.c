#include "assembler.h"
#define MINIMUM_NUMBER_OF_ARGS 2
#define FIRST_ADDRESS_FOR_IC 100
#define FIRST_ADDRESS_FOR_DC 0
int main(int argc, char *argv[])
{
    int i;
    if (argc < MINIMUM_NUMBER_OF_ARGS)
    {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }
    /*Loop over all file name paramaters, execute the assembler for each of them*/
    for (i = 1; i < argc; i++)
    {
        /*Intialize instructions and data counters*/
        /*For each file*/
        int IC = FIRST_ADDRESS_FOR_IC;
        int DC = FIRST_ADDRESS_FOR_DC;
        int is_error = FALSE;
        /*Data Structures*/
        /*Initalize all linked lists to be passed to the functions*/
        MacroNode *macro_list = init_macro_table();
        BinaryNode *code_image = init_binary_image();
        BinaryNode *data_image = init_binary_image();
        Label *label_list = init_label_table();
        if (!label_list || !data_image || !code_image || !macro_list)
        {
            printf("Error: Memory allocation failed\n");
            free_all(macro_list, code_image, data_image, label_list);
            return ERROR;
        }

        printf("----------- Processing file: \"%s\" -----------\n", argv[i]);

        if (pre_assembler(argv[i], macro_list) == ERROR)
        {
            /*If pre assembler had an error or multiple errors, stop and move to next file*/
            printf("Pre Assembler Failed, Moving to next file.\n\n");
            /*removing invalid .am file created by the pre assembler*/
            remove_file(argv[i], POST_MACRO_EXT);
            free_all(macro_list, code_image, data_image, label_list);
            continue;
        }
        else
        {
            is_error = first_pass(argv[i], code_image, data_image, label_list, macro_list, &IC, &DC);

            /*If first pass had an error, pass it to second pass to prevent building the source files*/
            if (is_error != ERROR)
            { /*check if there was a critical error that cannot be continued to the second pass*/
                if (second_pass(argv[i], code_image, data_image, label_list, IC, DC, is_error) == ERROR)
                {
                    printf("Second Pass Failed, Moving to next file\n\n");
                    continue;
                }
                else
                {
                    printf("----------- Finished processing file: \"%s\" -----------\n\n", argv[i]);
                }
            }
        }
        /*Free all linked lists initialized. create new ones for the next file in the loop*/
        free_all(macro_list, code_image, data_image, label_list);
    }

    return 0;
}

void free_all(MacroNode *macro_list, BinaryNode *code_image, BinaryNode *data_image, Label *label_list)
{
    free_binary_image(code_image);
    free_binary_image(data_image);
    free_label_table(label_list);
    free_macro_table(macro_list);
}