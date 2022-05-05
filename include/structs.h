#ifndef STRUCTS_H
#define STRUCTS_H

#define INT 1
#define STRING 2


// eğer bir var değişkeninin calledı varse kullanıcı tanımlamıştır
struct Var
{
    int id;
    char * called;
    void * ptr;
    int type;
};
typedef struct Var Var;


struct Token
{
    char * name;
    Var * vars;
    int var_count;
};
typedef struct Token Token;

struct Tokenlist
{
    int token_count;
    Token * tokens;
};
typedef struct Tokenlist Tokenlist;


#endif