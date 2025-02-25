#include "assembler.h"
#include "preasm.h"

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
        if (pre_assembler(argv[i]) == ERROR)
        {
            printf("Error: pre assmebler failed\n");
            return 1;
        }
        }

    return 0;
}
