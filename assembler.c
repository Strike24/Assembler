#include "assembler.h"

void free_all(MacroNode *macro_list, BinaryNode *code_image, BinaryNode *data_image, Label *label_list)
{
    free_binary_image(code_image);
    free_binary_image(data_image);
    free_label_table(label_list);
    free_macro_table(macro_list);
}

int main(int argc, char *argv[])
{
    int i;
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        MacroNode *macro_list = init_macro_table();
        BinaryNode *code_image = init_binary_image();
        BinaryNode *data_image = init_binary_image();
        Label *label_list = init_label_table();
        int IC = 100;
        int DC = 0;
        int is_error = FALSE;
        printf("----------- Processing file: \"%s\" -----------\n", argv[i]);
        if (pre_assembler(argv[i], macro_list) == ERROR)
        {
            printf("Pre Assembler Failed, Exiting Program.\n");
            remove_file(argv[i], POST_MACRO_EXT);
            free_all(macro_list, code_image, data_image, label_list);
            return 1;
        }
        else
        {
            is_error = first_pass(argv[i], code_image, data_image, label_list, macro_list, &IC, &DC);

            if (second_pass(argv[i], code_image, data_image, label_list, IC, DC, is_error) == ERROR)
            {
                printf("Second Pass Failed, Exiting Program.\n");
                return 1;
            }
            else
            {
                printf("----------- Finished processing file: \"%s\" -----------\n\n", argv[i]);
            }
        }
        free_all(macro_list, code_image, data_image, label_list);
    }

    return 0;
}
