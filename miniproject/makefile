# Template makefile for small projects. 
SHELL = /bin/sh

prefix = .
exec_prefix = ${prefix}
srcdir = ${prefix}/src
bindir = ${exec_prefix}/bin

# Put your source files here
SRCS = commons.c main.c taskmanager.c threadpool.c moduleloader.c
OBJS = ${SRCS:%.c=${bindir}/%.o}

# Put libraries to be linked to your program
LIB_NAMES = pthread dl
LIBS = ${patsubst %,-l%,${LIB_NAMES}}
EXEC = scheduler

# List of dynamic modules
MODULES = printer_task
MODULE_OBJS = ${MODULES:%=${plugins}/%.so}

# Ustawienia kompilatora
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -Wl,--export-dynamic


.PHONY: all
all: ${bindir}/${EXEC} ${MODULE_OBJS}

${bindir}/${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	
${OBJS}: ${bindir}/%.o: ${srcdir}/%.c | ${bindir}
	${CC} ${CFLAGS} -c $^ -o $@
	
${bindir}:
	mkdir ${bindir}
	
${plugins}:
	mkdir ${plugins}
	
${MODULE_OBJS}: ${plugins}/%.so: ${srcdir}/%.c | ${plugins}
	${CC} ${CFLAGS} -c -fpic $^ -o ${plugins}/$*.o
	${CC} -shared -o $@ ${plugins}/$*.o
	
# Prints time usage
.PHONY: test
test:
	{ time ${bindir}/${EXEC}; } 2>&1
    
.PHONY: clean
clean:
	${RM} ${bindir}/${EXEC} ${OBJS} ${MODULE_OBJS}
	
