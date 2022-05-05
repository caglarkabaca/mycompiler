#include "./include/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * read_file(const char* path){
    FILE * fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char * buffer = (char *)malloc(sizeof(char) * (size + 1));
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

Tokenlist parser(const char * file)
{
    Tokenlist tokenlist;
    tokenlist.tokens = (Token *)malloc(sizeof(Token));
    tokenlist.token_count = 0;

    char buff[255];

    int buff_index = 0;
    int file_index = 0;

    int var_count = 0;

    while (file[file_index] != '\0')
    {
        memset(buff, 0, buff_index);
        buff_index = 0;

        int string_reading = 0;
        while ( (file[file_index] != ' ' || string_reading) && file[file_index] != '\n' && file[file_index] != '\0')
        {
            if (file[file_index] == COMMENT_CHAR && !string_reading)
            {
                while(file[file_index] != '\n' && file[file_index] != '\0')
                    file_index++;
                continue;
            }

            if (file[file_index] == STRING_CHAR)
                string_reading = !string_reading;

            buff[buff_index] = file[file_index];
            file_index++;
            buff_index++;
        }


        char * word = (char *)calloc(1, sizeof(char) * (buff_index + 1));
        strcat(word, buff);
        file_index++;

        if (strlen(word) < 1)
            continue;

        switch(word[0]) // ilk karakter
        {
            case STRING_CHAR: ;
                word = word + 1;
                word[buff_index - 2] = '\0'; 
                tokenlist.tokens[tokenlist.token_count - 1].var_count++;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)realloc(tokenlist.tokens[tokenlist.token_count - 1].vars, sizeof(Var) * tokenlist.tokens[tokenlist.token_count - 1].var_count);
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].type = STRING;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = var_count;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = 0;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = (void *)word;
                var_count++;
                break;

            case VAR_CHAR: ;
                word = word + 1;
                tokenlist.tokens[tokenlist.token_count - 1].var_count++;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)realloc(tokenlist.tokens[tokenlist.token_count - 1].vars, sizeof(Var) * tokenlist.tokens[tokenlist.token_count - 1].var_count);
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = var_count;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = (void *)word;
                var_count++;
                break;

            case NUMBER_CHAR: ;
                int * number = (int *)malloc(sizeof(int));
                word = word + 1;
                int mul = 1;
                if (word[0] == '-')
                {
                    mul *= -1;
                    word = word + 1;
                }
                for (int a = strlen(word) - 1; a >= 0; a--)
                {
                    *number += mul * (word[a] - '0');
                    mul *= 10;
                }
                tokenlist.tokens[tokenlist.token_count - 1].var_count++;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)realloc(tokenlist.tokens[tokenlist.token_count - 1].vars, sizeof(Var) * tokenlist.tokens[tokenlist.token_count - 1].var_count);
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = var_count;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].type = INT;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = (void *)malloc(sizeof(int *));
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = number;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = 0;
                var_count++;
                break;

            default: // kelime
                tokenlist.token_count++;
                tokenlist.tokens = (Token *)realloc(tokenlist.tokens, sizeof(Token) * tokenlist.token_count);
                tokenlist.tokens[tokenlist.token_count - 1].name = (char *)calloc(1, sizeof(char) * (buff_index + 1));
                strcat(tokenlist.tokens[tokenlist.token_count - 1].name, word);
                tokenlist.tokens[tokenlist.token_count - 1].var_count = 0;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)malloc(sizeof(Var));
                break;
        }

        if (file[file_index] == '\0')
            break;

    }

    return tokenlist;
}