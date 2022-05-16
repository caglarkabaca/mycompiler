all: help

compile:
	gcc -g -o mycompiler main.c parser.c compiler.c -lm

run:
	./mycompiler --compile ./test.mc

machine: compile
	./mycompiler --machine ./test.mc

help: compile
	./mycompiler --help

debug:
	gdb --args ./mycompiler --compile ./test.mc