#include "assembler.h"

int main(int argc, char *argv[])
{
    BinaryNode *code_image = init_binary_image();
    BinaryNode *data_image = init_binary_image();
    Label *label_list = init_label_table();
    int i;
    int IC = 100;
    int DC = 0;
    int is_error = FALSE;

    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        printf("----------- Processing file: \"%s\" -----------\n", argv[i]);
        if (pre_assembler(argv[i]) == ERROR)
        {
            printf("Pre Assembler Failed, Exiting Program.\n");
            remove_file(argv[i], POST_MACRO_EXT);
            return 1;
        }
        else
        {
            is_error = first_pass(argv[i], code_image, data_image, label_list, &IC, &DC);

            if (second_pass(argv[i], code_image, data_image, label_list, IC, DC) == ERROR)
            {
                printf("Second Pass Failed, Exiting Program.\n");
                return 1;
            }
            else
            {
                printf("----------- Finished processing file: \"%s\" -----------\n\n", argv[i]);
            }
        }
    }

    free_binary_image(code_image);
    free_binary_image(data_image);
    free_label_table(label_list);
    return 0;
}
