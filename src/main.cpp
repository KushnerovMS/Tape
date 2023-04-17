#include <fstream>
#include <iostream>
#include <chrono>
#include <exception>
#include <random>

#include "Tape/Error.h"
#include "Tape/Tape.h"
#include "Tape/Sort.h"

struct
{
    bool operator()(int a, int b) { return a < b; }
} less;

int main() try
{
    std::string inputFileName;
    std::string outputFileName;

    std::cout << "Input file name:" << std::endl;
    std::cin >> inputFileName;
    std::cout << "Output file name:" << std::endl;
    std::cin >> outputFileName;

    Tape::Tape<int> inTape(inputFileName.c_str());
    Tape::Tape<int> outTape = Tape::CreateTape<int>(outputFileName.c_str(), inTape.getSize());
    Tape::NaturalSorting(inTape, outTape, less);

    return 0;
}
catch(Tape::Error& err)
{
    std::cout << "Tape error: " << err.what() << std::endl;
}
catch(std::exception& exp)
{
    std::cout << "Standart error: " << exp.what() << std::endl;
}
catch(...)
{
    std::cout << "Unknown error\n";
}
