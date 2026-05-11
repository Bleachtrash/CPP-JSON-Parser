#include "src/json.hpp"
#include <string>
#include <vector>
#include "src/Tokenizer.hpp"

JSON& JSON::operator[](std::string key)
{
    return this->json[key];
}

JSON& JSON::operator =(std::string val)
{
    this->j_type = j_string;
    this->vals.s = val;
    return *this;
}
JSON& JSON::operator=(float val)
{
    this->j_type = j_float;
    this->vals.f = val;
    return *this;
}

std::string JSON::toString()
{
    return this->vals.s;
}
float JSON::toFloat()
{
    return this->vals.f;
}

void JSON::parse(std::string json_str)
{
    this->parse(new Tokenizer(json_str.c_str()));
}


void parse_list(Tokenizer* t, JSON& json)
{

}

void parse_key_value_pair(Tokenizer* t, JSON& json)
{
    std::string key = t->token;
    JSON value;
    t->next_token();
    if(t->is_done())
        return;
    if(t->token[0] != '\"')
    {
        printf("No closing \"!\n");
        return;
    }
    t->next_token();
    if(t->token[0] != ':')
        return;
    t->next_token();
    switch (t->token[0])
    {
    case '\"':
        // Parse a string
        value.j_type = value.j_string;
        t->next_token();
        while(t->token[0] != '\"')
        {
            value.vals.s += t->token;
            value.vals.s += " ";
            t->next_token();
        }
        break;
    case '[':
        // Parse a list
        value.j_type = value.j_list;
        parse_list(t, json);
        break;
    case '{':
        value.j_type = value.j_json;
        value.parse(t);
        break;
    default:
        // Parse a number
        value.j_type = value.j_float;
        value.vals.f = std::stof(t->token);
        break;
    }
    json[key] = value;
    t->next_token();
}

void JSON::parse(Tokenizer* t)
{
    if(t->token[0] == '{')
    {
        // We are looking at JSON
        // In a while loop maybe??
        t->next_token();
        while(t->token[0] == '\"')
        {
            t->next_token();
            if(t->is_done())
                break;
            
            parse_key_value_pair(t, *this);

            if(t->is_done() || t->token[0] == '}')
                break;
            t->next_token();
        }
    }
}