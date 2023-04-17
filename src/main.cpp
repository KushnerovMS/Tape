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
    std::default_random_engine rnd(std::time(nullptr));
    std::uniform_int_distribution<int> dstr (0, 100000);

    size_t tapeSize = 100000;
    Tape::Tape<int> inTape = Tape::CreateTape<int>("InTape", tapeSize);
    do inTape.setItem(dstr(rnd));
    while (inTape.Next());
    std::cout << inTape << std::endl << std::endl;

    Tape::Tape<int> outTape = Tape::CreateTape<int>("OutTape", tapeSize);

    Tape::NaturalSorting(inTape, outTape, less);

    std::cout << outTape << std::endl;

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
