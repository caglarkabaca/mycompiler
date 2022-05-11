#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/structs.h"
#include "./include/parser.h"
#include "./include/compiler.h"

void print_tokenlist(Tokenlist tokenlist)
{   
    printf("-- tokenlist --\n");
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
    }
    return 0;
}