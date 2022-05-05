all: docker

compile:
	gcc-9 -g -o mycompiler main.c parser.c compiler.c

run:
	./mycompiler --compile ./test.mc

docker:
	docker build -t myc ./
	docker run myc