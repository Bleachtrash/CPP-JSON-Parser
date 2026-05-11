#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
struct JSON
{
    std::unordered_map<std::string, JSON> json;
    enum
    {
        j_string,
        j_float,
        j_list,
        j_json,
        j_null,
    } j_type;
    struct Vals
    {
        float fl;
        std::string str;
        std::vector<JSON> list;
        // JSON *json;
    } vals;

    JSON& operator [](std::string key);
    JSON& operator =(std::string val);
    JSON& operator =(float val);
    std::string s();
    float f();
    std::vector<JSON> l();
    // JSON j();
    void parse(std::string json_str);
    void parse(Tokenizer* t);
    void parse_from_file(const char* file);
};