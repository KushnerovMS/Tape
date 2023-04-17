#pragma once

#include <fstream>
#include <chrono>

namespace Tape {

struct Config
{
    std::chrono::microseconds ReadWriteTime = std::chrono::microseconds(0);
    std::chrono::microseconds RewindTime = std::chrono::microseconds(0);
    std::chrono::microseconds SeekTime = std::chrono::microseconds(0);

    Config(const char* fileName);
};

extern Config config;

} // namesapce Tape
