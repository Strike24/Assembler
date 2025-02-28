GCCFLAGS = gcc -ansi -pedantic -g -Wall

assembler: assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o
	${GCCFLAGS} -ansi -pedantic -g -Wall -o assembler assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o

assembler.o: assembler.c
	${GCCFLAGS} -c assembler.c
	
preasm.o: preasm.c
	${GCCFLAGS} -c preasm.c

filefunctions.o: filefunctions.c
	${GCCFLAGS} -c filefunctions.c

macrofunctions.o: macrofunctions.c
	${GCCFLAGS} -c macrofunctions.c

labelsfunctions.o: labelsfunctions.c
	${GCCFLAGS} -c labelsfunctions.c

operations.o: operations.c
	${GCCFLAGS} -c operations.c

firstpass.o: firstpass.c
	${GCCFLAGS} -c firstpass.c

clean:
	rm -f *.o assembler