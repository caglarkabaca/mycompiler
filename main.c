#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/structs.h"
#include "./include/parser.h"
#include "./include/compiler.h"

void print_tokenlist(Tokenlist tokenlist)
{   
    printf("-- tokenlist(%d) --\n", tokenlist.token_count);
    for (int i = 0; i < tokenlist.token_count; i++)
    {
        printf("%d token name: %s", tokenlist.tokens[i].var_count, tokenlist.tokens[i].name);
        for (int j = 0; j < tokenlist.tokens[i].var_count; j++)
        {

            if (tokenlist.tokens[i].vars[j].called)
            {
                printf(" c:%s(%d)", (char *)tokenlist.tokens[i].vars[j].called, tokenlist.tokens[i].vars[j].id);
            }
            else if (tokenlist.tokens[i].vars[j].type == STRING)
            {
                printf(" s:%s(%d)", (char *)tokenlist.tokens[i].vars[j].ptr, tokenlist.tokens[i].vars[j].id);
            }
            else if (tokenlist.tokens[i].vars[j].type == INT)
            {
                int number = 0;
                number = *(int *)tokenlist.tokens[i].vars[j].ptr;
                printf(" i:%d(%d)", number, tokenlist.tokens[i].vars[j].id);
            }
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    printf("... main running ...\n");
    if (argc >= 3 && (strcmp(argv[1], "--compile") == 0))
    {
        const char * file = read_file(argv[2]);
       
        Tokenlist tokenlist = parser(file);
        print_tokenlist(tokenlist);

        printf("-- output --\n");
        compile(tokenlist);

        free(tokenlist.tokens);

    }
    else if (argc >= 3 && (strcmp(argv[1], "--machine") == 0))
    {
        const char * file = read_file(argv[2]);

        Tokenlist tokenlist = parser(file);
        print_tokenlist(tokenlist);

        printf("-- machine code --\n");
        char * machine_code = to_machine_code(file, tokenlist);
        printf("%s", machine_code);

        char * name_file = (char *)calloc(strlen(argv[2] + 2), sizeof(char));
        strcpy(name_file, argv[2]);
        name_file[strlen(name_file) - 1] = 'a';
        name_file[strlen(name_file)] = 'c';
        name_file[strlen(name_file) + 1] = '\0';
        FILE *fp = fopen(name_file, "w");
        fputs(machine_code, fp);
        fclose(fp);
    }
    return 0;
}