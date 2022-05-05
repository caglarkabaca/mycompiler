#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/structs.h"
#include "./include/parser.h"

int main(int argc, char** argv)
{
    printf("... main running ...\n");
    if (argc == 3 && (strcmp(argv[1], "--compile") == 0))
    {
        const char * file = read_file(argv[2]);

        Tokenlist tokenlist = parser(file);

        for (int i = 0; i < tokenlist.token_count; i++)
        {
            printf("%d token name: %s", tokenlist.tokens[i].var_count, tokenlist.tokens[i].name);
            for (int j = 0; j < tokenlist.tokens[i].var_count; j++)
            {
                if (tokenlist.tokens[i].vars[j].called)
                {
                    printf(" %s", (char *)tokenlist.tokens[i].vars[j].called);
                }
                if (tokenlist.tokens[i].vars[j].type == STRING)
                {
                    printf(" %s", (char *)tokenlist.tokens[i].vars[j].ptr);
                }
                if (tokenlist.tokens[i].vars[j].type == INT)
                {
                    int number = 0;
                    number = *(int *)tokenlist.tokens[i].vars[j].ptr;
                    printf(" %d", number);
                }
            }
            printf("\n");
        }

    }
    return 0;
}