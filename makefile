assembler: assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o
	gcc -ansi -pedantic -g -Wall -o assembler assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o

assembler.o: assembler.c
	gcc -ansi -pedantic -g -Wall -c assembler.c
	
preasm.o: preasm.c
	gcc -ansi -pedantic -g -Wall -c preasm.c

filefunctions.o: filefunctions.c
	gcc -ansi -pedantic -g -Wall -c filefunctions.c

macrofunctions.o: macrofunctions.c
	gcc -ansi -pedantic -g -Wall -c macrofunctions.c

labelsfunctions.o: labelsfunctions.c
	gcc -ansi -pedantic -g -Wall -c labelsfunctions.c

operations.o: operations.c
	gcc -ansi -pedantic -g -Wall -c operations.c

firstpass.o: firstpass.c
	gcc -ansi -pedantic -g -Wall -c firstpass.c

