#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "Config.h"
#include "Error.h"

Tape::Config::Config(const char* fileName)
{
    std::fstream fstream(fileName, std::ios::in);
    if (fstream.is_open())
    {
        for (std::string str; std::getline(fstream, str); )
        {
            if (str.starts_with("ReadWriteTime "))
                ReadWriteTime   = std::chrono::microseconds( std::stoi(str.substr(sizeof("ReadWriteTime"))) );
            else if (str.starts_with("RewindTime "))
                RewindTime      = std::chrono::microseconds( std::stoi(str.substr(sizeof("RewindTime")))    );
            else if (str.starts_with("SeekTime "))
                SeekTime        = std::chrono::microseconds( std::stoi(str.substr(sizeof("SeekTime")))      );
            else if (str.starts_with("SortAvailableMemory "))
                SortAvailableMemory = std::stoul(str.substr(sizeof("SortAvailableMemory ")));
            else
            {
                std::stringstream strstr;
                strstr << "Unknown configuuration parameter \"" << str << '\"';
                throw Tape::Error(strstr.str().c_str());
            }
        }
    }
    else
    {
        fstream.clear();
        fstream.open(fileName, std::ios::out);

        fstream << "ReadWriteTime " << ReadWriteTime.count() << std::endl;
        fstream << "RewindTime " << RewindTime.count() << std::endl;
        fstream << "SeekTime " << SeekTime.count() << std::endl;
        fstream << "SortAvailableMemory " << SortAvailableMemory << std::endl;
    }
}

namespace Tape {
    Config config("Config");
}
