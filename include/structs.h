#ifndef STRUCTS_H
#define STRUCTS_H

#define COMMENT_CHAR '!'
#define STRING_CHAR '"'
#define NUMBER_CHAR '#'
#define VAR_CHAR '$'

#define INT 1
#define STRING 2

// opcodes
#define HLT 127
#define PRINT 1
#define PRINTLN 2
#define SET 3
#define ADD 4
#define SUB 5
#define MUL 6
#define DIV 7
#define GETINT 8
#define GETTXT 9
#define PUSH 10
#define SYSTEM 11
#define SETPOS 12
#define GOTO 13
#define GOGT 14
#define GOLT 15


// eğer bir var değişkeninin calledı varse kullanıcı tanımlamıştır
struct Var
{
    int id;
    char * called;
    void * ptr;
    int type;
};
typedef struct Var Var;

struct Varlist
{
    int var_count;
    Var * vars;
};
typedef struct Varlist Varlist;

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