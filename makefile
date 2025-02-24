assembler: assembler.c preasm.c filefunctions.c macrofunctions.c
	gcc -ansi -pedantic -g -Wall assembler.c preasm.c filefunctions.c macrofunctions.c -o assembler
