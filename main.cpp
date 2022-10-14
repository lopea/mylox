#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "scanner.h"
#include "Log.h"
#include "parser.h"
#include "Printer.h"


void LoadScript(const char* fileName)
{
    std::ifstream fileHandle(fileName, std::ios_base::in);
    if(!fileHandle)
    {
       std::cout <<  fileName << " cannot be open for writing" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::vector<char> data((std::istreambuf_iterator<char>(fileHandle)), (std::istreambuf_iterator<char>()));
    Scanner scanner(std::move(data));
    scanner.ScanTokens();
    Parser parser (scanner.Get());

    ExprPtr exp = parser.Parse();
    Printer p;
    std::cout << p.Print(exp) <<std::endl;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: mylox [script]" << std::endl;
        return EXIT_FAILURE;
    }
    else {
        LoadScript(argv[1]);
    }
    return EXIT_SUCCESS;
}
