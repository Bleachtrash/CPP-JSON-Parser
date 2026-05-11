#include "src/json.hpp"

int main()
{
    JSON j;

    j.parse("{ \" number1 \" : 10 , \" number2 \" : 100 , \" string1 \" : \" Hello There! \" }");
    // j.parse("{ \" string1 \" : \" Hello there! \" , \" number1 \" : 10.9 }");

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