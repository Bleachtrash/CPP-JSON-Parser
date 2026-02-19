#include <iostream>
#include <fstream>

using namespace std;
int main()
{
    const char* inFile = "test.json";
    const char* outFile = "JSON.hpp";

    ifstream in(inFile);
    ofstream out(outFile);

    string line;
    while(getline(in, line))
    {
        out << line << "\n";
    }
}