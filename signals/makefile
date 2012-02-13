# Template makefile for small projects. 
SHELL = /bin/sh

prefix = .
exec_prefix = ${prefix}
srcdir = ${prefix}/src
bindir = ${exec_prefix}/bin

# Put your source files here
SRCS = signals.c
OBJS = ${SRCS:%.c=${bindir}/%.o}

# Put libraries to be linked to your program
LIB_NAMES = 
LIBS = ${patsubst %,-l%,${LIB_NAMES}}

# Name of your executable, set by newproject.sh
EXEC = signals

CC = gcc

# Feel free to customize gcc & linker flags here
CFLAGS = -Wall -g -Os
LDFLAGS = 

.PHONY: all
all: ${bindir}/${EXEC}

${bindir}/${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	
${OBJS}: ${bindir}/%.o: ${srcdir}/%.c | ${bindir}
	${CC} ${CFLAGS} -c $^ -o $@
	
${bindir}:
	mkdir ${bindir}
	
# Prints time usage
.PHONY: test
test:
	{ time ${bindir}/${EXEC}; } 2>&1
    
.PHONY: clean
clean:
	${RM} ${bindir}/${EXEC} ${OBJS}
	
