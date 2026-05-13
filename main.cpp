#include "src/json.hpp"
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{

    JSON j;

    if(argc == 1)
        j.parse("{\"key\":\"val\"}");

    if(argc == 2)
    {
        j.parse_from_file(argv[1]);
    }

    j.print();

    return 0;
}