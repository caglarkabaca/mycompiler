all: help

compile:
	gcc -g -o uCompiler main.c parser.c compiler.c -lm

run:
	./uCompiler --compile ./test.mc

machine: compile
	./uCompiler --machine ./test.mc

help: compile
	./uCompiler --help

debug:
	gdb --args ./uCompiler --compile ./test.mc