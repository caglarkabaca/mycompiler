all: docker

win: compile-win run

compile-macos:
	gcc-9 -g -o mycompiler main.c parser.c compiler.c

compile-win:
	gcc -g -o mycompiler main.c parser.c compiler.c

run:
	./mycompiler --compile ./test.mc

docker:
	docker build -t myc ./
	docker run --interactive myc

debug:
	gdb --args ./mycompiler --compile ./test.mc