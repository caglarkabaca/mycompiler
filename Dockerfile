FROM ubuntu:20.04

RUN apt-get update && \
    apt-get -y install gcc mono-mcs && \
    rm -rf /var/lib/apt/lists/*

COPY . /Users/caglar/mycompiler
WORKDIR /Users/caglar/mycompiler

RUN gcc -o mycompiler main.c parser.c compiler.c
CMD ["./mycompiler", "--compile", "./test.mc"]
