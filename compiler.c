#include "./include/compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check(a, b) strcmp(a, b) == 0

int check_varlist(Varlist varlist, Var var) // -1 => bulunumadı | diğerleri indexi
{
    for (int i = 0; i < varlist.var_count; i++)
    {
        if (!varlist.vars[i].called)
            continue;

        if (check(varlist.vars[i].called, var.called))
        {
            //printf("l %d %p\n", i, varlist.vars[i].ptr);
            return i;
        }
    }
    return -1;
}

void compile(Tokenlist tokenlist)
{   
    Varlist varlist;
    
    varlist.vars = (Var *)malloc(sizeof(Var));
    varlist.var_count = 1;

    // SETPOS

    for (int _index = 0; _index < tokenlist.token_count; _index++)
    {
        Token token = tokenlist.tokens[_index];
        if (check(token.name, "SETPOS"))
        {
            Var var = token.vars[0];

            int index = check_varlist(varlist, var);

            if (index == -1)
            {
                index = varlist.var_count;
                varlist.var_count++;
                varlist.vars = (Var *)realloc(varlist.vars, sizeof(Var) * varlist.var_count);
                varlist.vars[index].called = (char *)calloc(1, sizeof(char)* strlen(var.called));
                strcpy(varlist.vars[index].called, var.called);
                varlist.vars[index].type = INT;
                int * number = (int *)malloc(sizeof(int));
                *number = _index;
                varlist.vars[index].ptr = (void *)malloc(sizeof(int *));
                varlist.vars[index].ptr = number;
            }
            else
                *(int *)varlist.vars[index].ptr = _index;
        }
    }
    
    // actual loop

    for (int token_i = 0; token_i < tokenlist.token_count; token_i++)
    {
        Token token = tokenlist.tokens[token_i];
        char * name = token.name;
        Var * vars = token.vars;
        int var_count = token.var_count;

        if (check(name, "HLT"))
            break; // eop
        
        if (check(name, "PRINT") || check(name, "PRINTLN"))
        {
            for (int var_i = 0; var_i < var_count; var_i++)
            {
                Var var = vars[var_i];

                if (var.called)
                {
                    int index = check_varlist(varlist, var);
                    var = varlist.vars[index];
                    if (var.type == INT)
                    {
                        int * number = (int *)malloc(sizeof(int));
                        *number = *(int *)var.ptr;
                        printf("%d", *number);
                        free(number);
                    }
                    else if (var.type == STRING) 
                    {
                        printf("%s", (char *)var.ptr);
                    }
                    continue;
                }

                if (var.type == INT)
                {
                    int number = 0;
                    number = *(int *)var.ptr;
                    printf("%d", number);
                }
                else if (var.type == STRING) 
                {
                    printf("%s", (char *)var.ptr);
                }
            }

            if (check(name, "PRINTLN"))
                printf("\n");
        }

        if (check(name, "SET"))
        {
            // index 0 , index 1 e eşitlenicek
            Var a = vars[0];
            Var b = vars[1];
            int index = check_varlist(varlist, a);

            if (index == -1)
            {
                index = varlist.var_count;
                varlist.var_count++;
                varlist.vars = (Var *)realloc(varlist.vars, sizeof(Var) * varlist.var_count);
                varlist.vars[index].called = (char *)calloc(1, sizeof(char)* strlen(a.called));
                strcpy(varlist.vars[index].called, a.called);
                varlist.vars[index].type = b.type;
                if (b.type == INT)
                {
                    int * number = (int *)malloc(sizeof(int));
                    *number = *(int *)b.ptr;
                    varlist.vars[index].ptr = (void *)malloc(sizeof(int *));
                    varlist.vars[index].ptr = number;
                }
                else if (b.type == STRING)
                {
                    varlist.vars[index].ptr = (void *)malloc(sizeof(char) * strlen(b.ptr));
                    strcpy(varlist.vars[index].ptr, b.ptr);
                }
            }
            else 
            {
                varlist.vars[index].type = b.type;
                if (b.type == INT)
                {
                    int * number = varlist.vars[index].ptr;
                    *number = *(int *)b.ptr;
                    varlist.vars[index].ptr = number;
                }
                else if (b.type == STRING)
                {
                    varlist.vars[index].ptr = (void *)realloc(varlist.vars[index].ptr, sizeof(char) * strlen(b.ptr));
                    strcpy(varlist.vars[index].ptr, b.ptr);
                }
            }
        }

        if (check(name, "ADD") || check(name, "SUB") || check(name, "MUL") || check(name, "DIV"))
        {
            Var toset = vars[0];
            Var a = vars[1];
            Var b = vars[2];

            if (a.called)
                a = varlist.vars[check_varlist(varlist, a)];

            if (b.called)
                b = varlist.vars[check_varlist(varlist, b)];

            int index = check_varlist(varlist, toset);

            if (index == -1)
            {
                index = varlist.var_count;
                varlist.var_count++;
                varlist.vars = (Var *)realloc(varlist.vars, sizeof(Var) * varlist.var_count);
                varlist.vars[index].called = (char *)calloc(1, sizeof(char)* strlen(toset.called));
                strcpy(varlist.vars[index].called, toset.called);
                varlist.vars[index].type = INT;
                varlist.vars[index].ptr = (void *)malloc(sizeof(int *));
            }

            if (check(name, "ADD"))
                *(int *)varlist.vars[index].ptr  = *(int *)a.ptr +  *(int *)b.ptr;
            else if (check(name, "SUB"))
                *(int *)varlist.vars[index].ptr  = *(int *)a.ptr -  *(int *)b.ptr;
            else if (check(name, "MUL"))
                *(int *)varlist.vars[index].ptr  = *(int *)a.ptr * *(int *)b.ptr;
            else if (check(name, "DIV"))
                *(int *)varlist.vars[index].ptr  = *(int *)a.ptr /  *(int *)b.ptr;
        }

        if (check(name, "GETINT"))
        {
            Var toset = vars[0];

            int index = check_varlist(varlist, toset);

            if (index == -1)
            {
                index = varlist.var_count;
                varlist.var_count++;
                varlist.vars = (Var *)realloc(varlist.vars, sizeof(Var) * varlist.var_count);
                varlist.vars[index].called = (char *)calloc(1, sizeof(char)* strlen(toset.called));
                strcpy(varlist.vars[index].called, toset.called);
                varlist.vars[index].type = INT;
                varlist.vars[index].ptr = (void *)malloc(sizeof(int *));
            }

            int data;
            fflush(stdout);
            if (scanf("%d", &data) == -1)
                perror("scanf failed");
            *(int *)varlist.vars[index].ptr = data;
        }

        if (check(name, "GOTO") || check(name, "GOGT") || check(name, "GOLT"))
        {
            Var loopvar = varlist.vars[check_varlist(varlist, vars[0])];

            if (check(name, "GOTO"))
                token_i = *(int *)loopvar.ptr;
            else 
            {
                Var a = vars[1];
                Var b = vars[2];

                if (a.called)
                    a = varlist.vars[check_varlist(varlist, a)];
                if (b.called)
                    b = varlist.vars[check_varlist(varlist, b)];            
                    
                if (check(name, "GOGT"))
                {
                    if (*(int *)a.ptr > *(int *)b.ptr)
                        token_i = *(int *)loopvar.ptr;   
                }
                else if (check(name, "GOLT"))
                {
                    if (*(int *)a.ptr < *(int *)b.ptr)
                        token_i = *(int *)loopvar.ptr;   
                }
            }
        }

    }
}