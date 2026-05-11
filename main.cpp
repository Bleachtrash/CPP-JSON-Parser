#include "src/json.hpp"
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
    JSON j;

    if(argc == 1)
        j.parse("{ \" number1 \" : 10 , \" number2 \" : 100 , \" string1 \" : \" Hello There! \" }");

    if(argc == 2)
    {
        // Load json file
        std::string json_string;
        std::ifstream JsonStream(argv[1], std::ios::in);
        if(JsonStream.is_open())
        {
            std::stringstream sstr;
            sstr << JsonStream.rdbuf();
            json_string = sstr.str();
            JsonStream.close();
        }
        j.parse(json_string);
    }

    for(auto i : j.json)
    {
        printf("%s: ", i.first.c_str());
        if(i.second.j_type == i.second.j_float)
            printf("%0.2f\n", i.second.vals.f);
        else if(i.second.j_type == i.second.j_string)
            printf("%s\n", i.second.vals.s.c_str());
    }

    return 0;
}