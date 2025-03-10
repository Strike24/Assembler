#include "assembler.h"

int main(int argc, char *argv[])
{
    BinaryNode *code_image = init_binary_image();
    BinaryNode *data_image = init_binary_image();
    Label *label_list = init_label_table();
    int i;

    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        /*
        if (pre_assembler(argv[i]) == ERROR)
        {
            printf("Error: pre assmebler failed\n");
            return 1;
        }
            */

        if (first_pass(argv[i], code_image, data_image, label_list) == ERROR)
        {
            printf("Error: assembler failed\n");
            return 1;
        }
    }

    free_binary_image(code_image);
    free_binary_image(data_image);
    free_label_table(label_list);
    return 0;
}
