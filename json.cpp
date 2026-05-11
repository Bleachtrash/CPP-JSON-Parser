#include "src/json.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "src/Tokenizer.hpp"

JSON& JSON::operator[](std::string key)
{
    return this->json[key];
}

JSON& JSON::operator =(std::string val)
{
    this->j_type = j_string;
    this->vals.str = val;
    return *this;
}
JSON& JSON::operator=(float val)
{
    this->j_type = j_float;
    this->vals.fl = val;
    return *this;
}

std::string JSON::s()
{
    return this->vals.str;
}
float JSON::f()
{
    return this->vals.fl;
}
std::vector<JSON> JSON::l()
{
    return this->vals.list;
}

JSON JSON::j()
{
    return *(this->vals.json);
}

void JSON::parse(std::string json_str)
{
    this->parse(new Tokenizer(json_str.c_str()));
}


void parse_list(Tokenizer* t, JSON& json);

JSON parse_value(Tokenizer* t)
{
    JSON json;
    switch (t->token[0])
    {
    case '\"':
        // Parse a string
        json.j_type = json.j_string;
        t->next_token();
        while(t->token[0] != '\"')
        {
            json.vals.str += t->token;
            t->next_token();
        }
        break;
    case '[':
        // Parse a list
        json.j_type = json.j_list;
        t->next_token();
        parse_list(t, json);
        break;
    case '{':
        json.j_type = json.j_json;
        json.parse(t);
        break;
    default:
        // Parse a number
        json.j_type = json.j_float;
        json.vals.fl = std::stof(t->token);
        break;
    }
    return json;
}
void parse_value(Tokenizer *t, JSON& json)
{
    json = parse_value(t);
}

void parse_list(Tokenizer* t, JSON& json)
{
    while(t->token[0] != ']')
    {
        json.vals.list.push_back(parse_value(t));
        t->next_token();
        if(t->token[0] == ']')
            break;
        t->next_token();
    }
}

void parse_key_value_pair(Tokenizer* t, JSON& json)
{
    std::string key = t->token;
    JSON value;
    t->next_token();
    if(t->is_done())
        return;

    t->next_token();
    t->next_token();

    json[key] = parse_value(t);

    t->next_token();
}

void JSON::parse(Tokenizer* t)
{
    if(t->token[0] == '{')
    {
        // We are looking at JSON
        t->next_token();
        while(true)
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

std::string remove_space(std::string &str)
{
    std::string clean;
    for(char c : str)
    {
        if(!isspace(c))
        {
            clean += c;
        }
    }
    str = clean;
    return clean;
}

bool is_special(const char c)
{
    return c == '{' || c == '}' || c == '\"' || c == ':' || c == ',' || c == '[' || c == ']';
}

std::string clean(std::string str)
{
    std::string clean_json;
    for(int i = 0; i < str.length(); i++)
    {
        std::string add;

        if(is_special(str[i]))
        {
            if(i > 0 && str[i-1] != ' ')
                add += ' ';
            add+= str[i];
            if(i < str.length() && str[i+1] != ' ')
                add += ' ';
        }
        // else if(str[i] == '\n')
        //     break;
        else
            add += str[i];

        clean_json += add;
    }
    return clean_json;
}

std::string read_json_file(const char* file)
{
    // Load json file
    std::string json_string;
    std::ifstream JsonStream(file, std::ios::in);
    if(JsonStream.is_open())
    {
        std::stringstream sstr;
        sstr << JsonStream.rdbuf();
        json_string = sstr.str();
        JsonStream.close();
    }
    remove_space(json_string);
    return clean(json_string);
}

void JSON::parse_from_file(const char* file)
{
    this->parse(read_json_file(file));
}