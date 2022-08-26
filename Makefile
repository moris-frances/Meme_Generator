CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter 
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
EXENAME = MemeGenerator

${EXENAME}: ${CFILES} ${HFILES}
	${CC} ${CFLAGS} ${CFILES} ${HFILES} -lm -o ${EXENAME}
clean: ${EXENAME}
	rm -f ${EXENAME}.o ${EXENAME}
	echo Clean done
