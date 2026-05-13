#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>


struct Tokenizer
{
    std::string remaining_str;
    std::string token;
    std::string deliminator;

    Tokenizer() : Tokenizer(" ") {}
    Tokenizer(std::string str) : Tokenizer(str, " ") {}
    Tokenizer(std::string str, std::string deliminator)
    {
        this->remaining_str = str;
        this->deliminator = deliminator;
        this->next_token();
    }

    void next_token()
    {
        if(this->remaining_str == "")
        {
            this->token = "";
            return;
        }
        auto pos = this->remaining_str.find_first_of(this->deliminator);
        if(pos == std::string::npos)
        {
            this->token = this->remaining_str;
            this->remaining_str = "";
            return;
        }
        this->token = this->remaining_str.substr(0, pos);

        this->remaining_str = this->remaining_str.substr(pos+1, this->remaining_str.size());
    }
    bool is_done()
    {
        return this->token.size() == 0;
    }
};