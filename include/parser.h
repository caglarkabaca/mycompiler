#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

char * read_file(const char* path);

Tokenlist parser(const char* file);

char * to_machine_code(const char* file, Tokenlist tokenlist);

#endif