#include "src/json.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "src/Tokenizer.hpp"

JSON::JSON()
{
    this->j_type = j_null;
}
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
bool JSON::b()
{
    return this->vals.fl;
}
std::vector<JSON> JSON::l()
{
    return this->vals.list;
}


void JSON::parse(std::string json_str)
{
    this->parse(new Tokenizer(json_str, ""));
}

std::string trim(std::string str)
{
    auto start = str.find_first_not_of(" ");
    if(start == std::string::npos)
        return "";
    auto end = str.find_last_not_of(" ");

    return str.substr(start, end+1);
}

JSON parse_collection(Tokenizer* t, JSON& json);
JSON parse_list(Tokenizer* t, JSON& json);

void JSON::parse(Tokenizer *t)
{
    if(t->is_done())
        return;
    if(trim((std::string)t->token)[0] == '{')
    {
        // We a looking at a collection
        this->j_type = j_collection;

        std::string token = t->token.substr(1, t->token.size());

        Tokenizer *collection_t = new Tokenizer(token, ",");
        parse_collection(collection_t, *this);
        return;
    }
    if(trim((std::string)t->token)[0] == '[')
    {
        // We are looking at a list
        this->j_type = j_list;

        std::string token = t->token.substr(1, t->token.size());
        Tokenizer* list_t = new Tokenizer(token, ",");
        parse_list(list_t, *this);
        return;
    }
    if(trim((std::string)t->token)[0] == '\"')
    {
        // We are looking at a string
        this->j_type = j_string;

        std::string val = t->token;
        val = trim(val);
        val = val.substr(1, val.size()-2);
        this->vals.str = val;
        return;
    }
    if(trim(t->token) == "true")
    {
        this->j_type = j_bool;
        this->vals.fl = true;
        return;
    }
    if(trim(t->token) == "false")
    {
        this->j_type = j_bool;
        this->vals.fl = false;
        return;
    }
    if(trim(t->token) == "null")
        return;
    // Parse a float
    this->j_type = j_float;
    this->vals.fl = std::stof(t->token);
}

std::string tokenize_collection(Tokenizer* t)
{
    int bracket_count = 0;
    std::string res;

    while(!t->is_done())
    {
        std::string token = t->token;
        token = trim(token);
        bracket_count += token[0] == '{';
        bracket_count -= token[token.size()-1] == '}';
        res += token;
        res += ':';
        if(bracket_count <= 0)
            break;
        t->next_token();
    }

    res  = res.substr(0, res.size()-(1 + (res[res.size()-2] == '}')));
    
    return res;
}

JSON parse_key_value_pair(Tokenizer* t, JSON& json)
{
    std::string token = trim((std::string)t->token);

    std::string key = token;

    token = t->token;
    token = trim(token);
    t->next_token();
    token = tokenize_collection(t);
    Tokenizer* val_t = new Tokenizer(token, "");
    JSON val;
    val.parse(val_t);

    json[key] = val;
    return json;
}

JSON parse_collection(Tokenizer* t, JSON& json)
{
    // t.deliminator = ','
    while(!t->is_done())
    {
        
        if(t->token[t->token.size()-1] == '}')
            t->token = t->token.substr(0, t->token.size()-1);
        std::string token = t->token;
        token = trim(token);

        Tokenizer *key_value_t = new Tokenizer(token, ":");

        parse_key_value_pair(key_value_t, json);

        t->next_token();
    }
    return json;
}

std::string tokenize_list(Tokenizer* t)
{
    int bracket_count = 0;
    std::string res;

    while(!t->is_done())
    {
        std::string token = t->token;
        token = trim(token);
        bracket_count += token[0] == '[';
        bracket_count -= token[token.size()-1] == ']';
        res += token;
        res += ',';
        if(bracket_count <= 0)
            break;
        t->next_token();
    }

    res  = res.substr(0, res.size()-(1+ (res[res.size()-2] == ']')));
    
    return res;
}

JSON parse_list(Tokenizer* t, JSON& json)
{
    // t.deliminator = ','
    while(!t->is_done() && t->token != "]")
    {
        std::string token = tokenize_list(t);
        Tokenizer* val_t = new Tokenizer(token, "");
        JSON val;
        val.parse(val_t);

        json.vals.list.push_back(val);

        t->next_token();
    }
    return json;
}

std::string clean_str(std::string str)
{
    std::string clean;
    for(char c : str)
    {
        if(c != '\n')
        {
            clean += c;
        }
    }
    str = clean;
    return clean;
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
    return clean_str(json_string);
}

void JSON::parse_from_file(const char* file)
{
    this->parse(trim(read_json_file(file)));
}

void JSON::print()
{
    switch (this->j_type)
    {
    case j_float:
        printf("%0.2f", this->f());
        break;
    case j_string:
        printf("\"%s\"", this->s().c_str());
        break;
    case j_bool:
        printf("%s", this->b()? "true" : "false");
        break;
    case j_list:
        printf("[ ");
        for(int i = 0; i < this->l().size(); i++)
        {
            this->l()[i].print();
            printf(", ");
        }
        printf("\b\b ]");
        break;
    case j_collection:
        printf("{ ");
        for(auto i : this->json)
        {
            printf("%s: ", i.first.c_str());
            i.second.print();
            printf(", ");
        }
        printf("\b\b }");
        break;
    case j_null:
        printf("null");
        break;
    default:
        break;
    }
}

// int main()
// {
//     JSON j;
//     j.parse_from_file("test.json");
//     printf("%i\n", j.j_type);
//     return 0;
// }