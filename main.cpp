#include "src/json.hpp"
#include <fstream>
#include <sstream>
#include <string>

void printJSON(JSON j);

void printVal(JSON j)
{
    if(j.j_type == j.j_float)
        printf("%0.2f", j.f());
    else if(j.j_type == j.j_string)
        printf("\"%s\"", j.s().c_str());
    else if(j.j_type == j.j_list)
    {
        printf("[ ");
        for(int i = 0; i < j.l().size(); i++)
        {
            printf("%i: ", i);
            printVal(j.l()[i]);
            printf(", ");
        }
        printf("\b\b ]");
    }
    else if(j.j_type == j.j_json)
    {
        printJSON(j.j());
    }
}
void printJSON(JSON j)
{
    printf("{\n");
    for(auto i : j.json)
    {
        printf("   %s: ", i.first.c_str());
        printVal(i.second);
        printf(",\n");
    }
    printf("}");
}

int main(int argc, char* argv[])
{
    JSON j;

    if(argc == 1)
        j.parse("{ \" list \" : [ 1 , 2 , 3 ] }");

    if(argc == 2)
    {
        j.parse_from_file(argv[1]);
    }

    printJSON(j);

    return 0;
}