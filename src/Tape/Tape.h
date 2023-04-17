#pragma once

#include <ostream>
#include <stdexcept>
#include <ios>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

#include "Error.h"
#include "Config.h"

namespace Tape {

template<typename T>
class Tape
{
    std::fstream fileStream_;
    std::ios::pos_type endPos_ = 0;

    public:

    Tape (const char* tapeName) :
        fileStream_(tapeName, std::ios::in      |
                              std::ios::out     |
                              std::ios::binary  |
                              std::ios::ate)
    {
        if (! fileStream_.is_open())
        {
            std::stringstream strstr;
            strstr << "No file with name \"" << tapeName << '\"';
            throw Error(strstr.str().c_str());
        }

        endPos_ = fileStream_.tellp();
        if (endPos_ == 0)
            throw Error("Can not be tape with 0 size");

        if (endPos_ % sizeof(T) != 0)
            throw Error("Size of file is not multiple of type size");

        fileStream_.seekp(0);
    }

    int getSize() const { return endPos_ / sizeof(T); }

    T getItem()
    {
        std::this_thread::sleep_for(config.ReadWriteTime);

        T res = 0;
        fileStream_.read(reinterpret_cast<char*>(&res), sizeof(T));
        fileStream_.seekg(-sizeof(T), std::ios::cur);

        if (fileStream_.fail())
            throw Error("Failed to get item");

        return res;
    }
    void setItem(const T& item)
    {
        std::this_thread::sleep_for(config.ReadWriteTime);

        fileStream_.write(reinterpret_cast<const char*>(&item), sizeof(T));
        fileStream_.seekg(-sizeof(T), std::ios::cur);

        if (fileStream_.fail())
            throw Error("Failed to set item");
    }

    bool Next()
    {
        if (fileStream_.tellp() + sizeof(T) == endPos_)
            return 0;

        std::this_thread::sleep_for(config.SeekTime);
        fileStream_.seekg(sizeof(T), std::ios::cur);

        return 1;
    }
    bool Prev()
    {
        if (fileStream_.tellp() == 0)
            return 0;

        std::this_thread::sleep_for(config.SeekTime);
        fileStream_.seekg(-sizeof(T), std::ios::cur);
        
        return 1;
    }

    void toBeg()
    {
        std::this_thread::sleep_for(config.RewindTime);
        fileStream_.seekg(0, std::ios::beg);
    }
    void toEnd()
    {
        std::this_thread::sleep_for(config.RewindTime);
        fileStream_.seekg(- sizeof(T), std::ios::end);
    }

    bool atBeg()
    {
        return fileStream_.tellp() == 0;
    }
    bool atEnd()
    {
        return fileStream_.tellp() == endPos_ - sizeof(T);
    }

};

template<typename T>
Tape<T> CreateTape(const char* tapeName, size_t size)
{
    std::ofstream stream(tapeName);
    if (stream.fail())
    {
        std::stringstream strstr;
        strstr << "File \"" << tapeName << "\" can not be created";
        throw Error(strstr.str().c_str());
    }
    std::vector<T> vec(size);
    stream.write(reinterpret_cast<char*>(vec.data()), size * sizeof(T));

    stream.close();

    return Tape<T>(tapeName);
}

template<typename T>
std::ostream& operator<< (std::ostream& stream, Tape<T>& tape)
{
    tape.toBeg();

    do  stream << tape.getItem() << ' ';
    while (tape.Next());

    return stream;
}

} // namespace Tape
