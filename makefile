assembler: assembler.c preproc.c
	gcc -ansi -pedantic -g -Wall assembler.c preproc.c -o assembler