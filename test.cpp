#include <stdio.h>
struct JSON
{
    char* key = "value";
    struct LIST
    {
        char* first = "list element";
        struct SECOND
        {
            char* key = "value";
            int number = 1;
        } second;
    } list; 
};

int main()
{
    JSON json;
    printf("%i\n", json.list.second.number);
    return 0;
}