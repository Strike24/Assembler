#include "assembler.h"
#include "preasm.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    pre_assembler(argv[1]);
    return 0;
}
