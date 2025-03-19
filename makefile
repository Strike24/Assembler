GCCFLAGS = gcc -ansi -pedantic -g -Wall

assembler: assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o binaryfunctions.o secondpass.o errors.o
	${GCCFLAGS} -ansi -pedantic -g -Wall -o assembler assembler.o preasm.o filefunctions.o macrofunctions.o labelsfunctions.o operations.o firstpass.o binaryfunctions.o secondpass.o errors.o

assembler.o: assembler.c assembler.h
	${GCCFLAGS} -c assembler.c
	
preasm.o: preasm.c preasm.h
	${GCCFLAGS} -c preasm.c

filefunctions.o: filefunctions.c filefunctions.h
	${GCCFLAGS} -c filefunctions.c

macrofunctions.o: macrofunctions.c macrofunctions.h
	${GCCFLAGS} -c macrofunctions.c

labelsfunctions.o: labelsfunctions.c labelsfunctions.h
	${GCCFLAGS} -c labelsfunctions.c

operations.o: operations.c operations.h
	${GCCFLAGS} -c operations.c

firstpass.o: firstpass.c firstpass.h
	${GCCFLAGS} -c firstpass.c

secondpass.o: secondpass.c secondpass.h
	${GCCFLAGS} -c secondpass.c

binaryfunctions.o: binaryfunctions.c binaryfunctions.h
	${GCCFLAGS} -c binaryfunctions.c

errors.o: errors.c errors.h
	${GCCFLAGS} -c errors.c

clean:
	rm -f *.o assembler