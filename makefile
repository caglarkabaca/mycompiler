all: docker

compile:
	gcc-7 -o mycompiler main.c parser.c compiler.c

run:
	./mycompiler --compile ./test.mc

docker:
	docker build -t myc ./
	docker run myc