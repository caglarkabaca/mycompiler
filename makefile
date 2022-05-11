all: compile run

compile:
	gcc-9 -g -o mycompiler main.c parser.c compiler.c

run:
	./mycompiler --compile ./test.mc

docker:
	docker build -t myc ./
	docker run --interactive myc

debug:
	gdb --args ./mycompiler --compile ./test.mc