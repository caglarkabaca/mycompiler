#include "./include/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char * read_file(const char* path){
    FILE * fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char * buffer = (char *)calloc(size + 2, sizeof(char));
    fread(buffer, 1, size, fp);
    buffer[size] = ' ';
    buffer[size + 1] = '\0';
    fclose(fp);
    
    int k = 0;
    for (int i = 0; buffer[i] != '\0'; ++i)
        if (buffer[i] != '\r')
            buffer[k++] = buffer[i];
    buffer[k] = '\0';
    
    return buffer;
}

int check_id(Tokenlist tokenlist, char * called, int var_count)
{
    int id = -1;

    for(int i = 0; i < tokenlist.token_count; i++)
    {
        for (int j = 0; j < tokenlist.tokens[i].var_count; j++)
        {
            char * name;
            if(tokenlist.tokens[i].vars[j].called)
                name = tokenlist.tokens[i].vars[j].called;
            else
                continue;;
            
            if (strcmp(name, called) == 0)
            {
                return tokenlist.tokens[i].vars[j].id;
            }
        }
    }
    return id;
}

Tokenlist parser(const char * file)
{
    Tokenlist tokenlist;
    tokenlist.tokens = (Token *)calloc(1, sizeof(Token));
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


        char * word = (char *)calloc(strlen(buff) + 1, sizeof(char));
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
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = -1;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = 0;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = (void *)word;
                break;

            case VAR_CHAR: ;
                word = word + 1;
                tokenlist.tokens[tokenlist.token_count - 1].var_count++;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)realloc(tokenlist.tokens[tokenlist.token_count - 1].vars, sizeof(Var) * tokenlist.tokens[tokenlist.token_count - 1].var_count);
                if (check_id(tokenlist, word, var_count) == -1)
                {
                    tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = var_count;
                    var_count++;
                }
                else
                {
                    tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = check_id(tokenlist, word, var_count);
                }
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = (void *)word;
                break;

            case NUMBER_CHAR: ;
                int * number = (int *)malloc(sizeof(int));
                *number = 0;
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
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].id = -1;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].type = INT;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = (void *)malloc(sizeof(int *));
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].ptr = number;
                tokenlist.tokens[tokenlist.token_count - 1].vars[tokenlist.tokens[tokenlist.token_count - 1].var_count - 1].called = 0;
                break;

            default: ; // kelime
                tokenlist.token_count++;
                tokenlist.tokens = (Token *)realloc(tokenlist.tokens, sizeof(Token) * tokenlist.token_count);
                tokenlist.tokens[tokenlist.token_count - 1].name = (char *)calloc(buff_index + 1, sizeof(char));
                strcat(tokenlist.tokens[tokenlist.token_count - 1].name, word);
                tokenlist.tokens[tokenlist.token_count - 1].var_count = 0;
                tokenlist.tokens[tokenlist.token_count - 1].vars = (Var *)calloc(1, sizeof(Var));
                break;
        }

        if (file[file_index] == '\0')
            break;

    }

    return tokenlist;
}

char * printbin(unsigned char val)
{
    char * binar = (char *)calloc(9, sizeof(char));
    char * a = (char *)calloc(2, sizeof(char));
    for(unsigned int i = 0x80; i; i >>= 1)
    {
        a[0] = val & i ? '1' : '0';
        strcat(binar, a);
    }
    return binar;
}

char * printbinvar(unsigned char val)
{
    char * binar = (char *)calloc(9, sizeof(char));
    char * a = (char *)calloc(2, sizeof(char));
    for(unsigned int i = 64; i; i >>= 1)
    {
        a[0] = val & i ? '1' : '0';
        strcat(binar, a);
    }
    return binar;
}

char * printbin32(int val)
{
    char * nums = (char *)calloc(33, sizeof(char));
    //printf("%d ", val);
    int i = 0;
    while (val != 0)
    {
        //printf("%c ", (val % 2) + '0');
        nums[i] = (val % 2) + '0';
        i++;
        val = val / 2;
    }
    //printf("> %s\n", nums);
    char temp;
    for (i = 0; i < strlen(nums)/2; i++)  
    {  
        temp = nums[i];  
        nums[i] = nums[strlen(nums) - i - 1];  
        nums[strlen(nums) - i - 1] = temp;  
    }
    if (strlen(nums) == 0)
        nums = "0";
    return nums;
}

int bintoint(char * opcode)
{
    int number = 0;
    for(int i = strlen(opcode); i > -1; i--)
    {
        if (opcode[i] == '1')
            number += pow(2, strlen(opcode) - i - 1);
    }
    return number;
}

unsigned char token_to_opcode(char * name)
{
    if (strcmp("HLT", name) == 0) return HLT;
    if (strcmp("PRINT", name) == 0) return PRINT;
    if (strcmp("PRINTLN", name) == 0) return PRINTLN;
    if (strcmp("SET", name) == 0) return SET;
    if (strcmp("ADD", name) == 0) return ADD;
    if (strcmp("SUB", name) == 0) return SUB;
    if (strcmp("MUL", name) == 0) return MUL;
    if (strcmp("DIV", name) == 0) return DIV;
    if (strcmp("GETINT", name) == 0) return GETINT;
    if (strcmp("GETTXT", name) == 0) return GETTXT;
    if (strcmp("PUSH", name) == 0) return PUSH;
    if (strcmp("SYSTEM", name) == 0) return SYSTEM;
    if (strcmp("SETPOS", name) == 0) return SETPOS;
    if (strcmp("GOTO", name) == 0) return GOTO;
    if (strcmp("GOGT", name) == 0) return GOGT;
    if (strcmp("GOLT", name) == 0) return GOLT;
}

char * opcode_to_string(int opcode)
{
    char * _opcode = (char *)calloc(255, sizeof(char));
    if (opcode == HLT) strcpy(_opcode, "HLT");
    if (opcode == PRINT) strcpy(_opcode, "PRINT");
    if (opcode == PRINTLN) strcpy(_opcode, "PRINTLN");
    if (opcode == SET) strcpy(_opcode, "SET");
    if (opcode == ADD) strcpy(_opcode, "ADD");
    if (opcode == SUB) strcpy(_opcode, "SUB");
    if (opcode == MUL) strcpy(_opcode, "MUL");
    if (opcode == DIV) strcpy(_opcode, "DIV");
    if (opcode == GETINT) strcpy(_opcode, "GETINT");
    if (opcode == GETTXT) strcpy(_opcode, "GETTXT");
    if (opcode == PUSH) strcpy(_opcode, "PUSH");
    if (opcode == SYSTEM) strcpy(_opcode, "SYSTEM");
    if (opcode == SETPOS) strcpy(_opcode, "SETPOS");
    if (opcode == GOTO) strcpy(_opcode, "GOTO");
    if (opcode == GOGT) strcpy(_opcode, "GOGT");
    if (opcode == GOLT) strcpy(_opcode, "GOLT");
    return _opcode;
}

char * to_machine_code(Tokenlist tokenlist)
{
    char * machine_code = (char *)calloc(1, sizeof(char));
    for (int i = 0; i < tokenlist.token_count; i++)
    {
        machine_code = (char *)realloc(machine_code, (strlen(machine_code) + 11) * sizeof(char));
        strcat(machine_code, printbin(token_to_opcode(tokenlist.tokens[i].name)));
        strcat(machine_code, " ");

        for (int j = 0; j < tokenlist.tokens[i].var_count; j++)
        {
            Var var = tokenlist.tokens[i].vars[j];

            if (var.id == -1)
            {
                if (var.type == INT)
                {
                    machine_code = (char *)realloc(machine_code, (strlen(machine_code) + 34) * sizeof(char));
                    strcat(machine_code, printbin32(*(int *)var.ptr));
                    strcat(machine_code, " ");
                    continue;
                }
                else if (var.type == STRING)
                {
                    int size = 8 * strlen((char *)var.ptr) + 3;
                    machine_code = (char *)realloc(machine_code, (strlen(machine_code) + size) * sizeof(char));
                    strcat(machine_code, "1");
                    for (int i = 0; i < strlen((char *)var.ptr); i++)
                    {
                        strcat(machine_code, printbin(((char *)var.ptr)[i]));
                    }
                    strcat(machine_code, " ");
                    continue;
                }
            }
            else 
            {
                machine_code = (char *)realloc(machine_code, (strlen(machine_code) + 34) * sizeof(char));
                strcat(machine_code, printbinvar(var.id));
                strcat(machine_code, " ");
            }
        }

        strcat(machine_code, "\n");
    }
    return machine_code;
}

char * from_machine_code(const char* file)
{
    
    char buff[255];
    int buff_index = 0;
    int file_index = 0;

    char * new_file = (char *)calloc(1, sizeof(char));

    while (file[file_index] != '\0')
    {
        memset(buff, 0, buff_index);
        buff_index = 0;
        
        while(file[file_index] != ' ' && file[file_index] != '\n' && file[file_index] != '\0')
        {
            buff[buff_index] = file[file_index];
            file_index++;
            buff_index++;
        }
        
        char * word = (char *)calloc(strlen(buff) + 1, sizeof(char));
        file_index++;
        

        for (int i = 0; i < strlen(buff); i++)
        {
            if (buff[i] == '1' || buff[i] == '0')
                word[i] = buff[i];
        }

        word[strlen(buff)] = '\0'; 

        if (strlen(word) < 1)
            continue;
        //printf("%s -> %d\n", word, strlen(word));

        if ((strlen(word) == 8) && word[0] == '0') // BUNLAR KOMUTLAR HLT,PRINT GIBI
        {
            char * _word = opcode_to_string(bintoint(word));
            new_file = realloc(new_file, (strlen(new_file) + strlen(_word) + 4) * sizeof(char));
            strcat(new_file, "\n");
            strcat(new_file, _word);
        }
        else if (strlen(word) == 7 && word[0] == '0') // BUNLAR DEĞİŞKENLER BİLEREK 7 OLMASI İÇİN AYARLANDILAR
        {
            char _word[255];
            sprintf(_word, "%d", bintoint(word));
            new_file = realloc(new_file, (strlen(new_file) + strlen(_word) + 4) * sizeof(char));
            strcat(new_file, " $");
            strcat(new_file, _word);
        }
        else if (strlen(word) >= 8 && word[0] == '1') // BUNLAR STRINGLER BAŞINDAKI 1 İ SİLİP STRING OKUYACAK
        { // string okuma lazım
            char * _word = (char *)calloc((strlen(word) / 3 ) + 1, sizeof(char));
            word = word + 1;

            char buff[9];
            for (int i = 0; i < strlen(word) / 3; i++)
            {
                buff[0] = word[0 + 8*i];
                buff[1] = word[1 + 8*i];
                buff[2] = word[2 + 8*i];
                buff[3] = word[3 + 8*i];
                buff[4] = word[4 + 8*i];
                buff[5] = word[5 + 8*i];
                buff[6] = word[6 + 8*i];
                buff[7] = word[7 + 8*i];

                char x[2];
                x[0] = bintoint(buff);
                strcat(_word, x);
            }

            new_file = realloc(new_file, (strlen(new_file) + strlen(word) + 5) * sizeof(char));
            strcat(new_file, " \"");
            strcat(new_file, _word);
            strcat(new_file, "\"");
        }
        else // KALANLAR İSE NORMAL TAM SAYILAR
        { 
            char _word[255];
            sprintf(_word, "%d", bintoint(word));
            new_file = realloc(new_file, (strlen(new_file) + strlen(_word) + 5) * sizeof(char));
            strcat(new_file, " #");
            strcat(new_file, _word);
        }

    }


    return new_file;
}