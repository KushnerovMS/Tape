#pragma once

namespace Tape {

class Error : public std::runtime_error
{
    public:
    Error(const char* msg) : std::runtime_error(msg) {}
};

} // namespace Tape
