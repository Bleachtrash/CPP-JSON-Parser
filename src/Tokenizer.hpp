#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>

struct Tokenizer
{
    char* remaining_str;
    char* token;
    char* deliminator;

    Tokenizer(const char* str) : Tokenizer(str, " ") {}
    Tokenizer() : Tokenizer(" ") {}
    
    Tokenizer(const char* str, const char* deliminator)
    {
        this->remaining_str = (char*)malloc(strlen(str)+1);
        strcpy(this->remaining_str, str);
        this->token = NULL;
        this->deliminator = (char*)malloc(strlen(deliminator)+1);
        strcpy(this->deliminator, deliminator);
        this->token = strtok(this->remaining_str, this->deliminator);
    }

    void next_token()
    {
        this->token = strtok(NULL, this->deliminator);
    }
    
    bool is_done()
    {
        return this->token == NULL;
    }
};