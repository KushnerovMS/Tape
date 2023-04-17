# About

This program emulate work with magnetic tape.

# Magnetic tape

Magnetic tape is type of container. There is magnetic head, that can write/read data from the cell, that is under head. Also head can move forward and backward along the tape, and rewind it to the beginning/ending of tape. All this operations require big amount of time, so it is not purposed to random data access.

# Dependences

CppCache requires Catch2 package to make tests. You can look https://github.com/catchorg/Catch2 for it.

# Building

For build you will need Cmake.

    mkdir build
    cd build
    cmake ..
    cmake --build .

Compilled program will be located in "build" directory.

# Making test

Go in directory build/tests and call `ctest`.

# Configuration

Program always creates configuration file in calling folder. By default delay of read/write, rewind and seek sets to zero. But you can edit file, and at the next execution program will read this parameters. Also you can change configuration inside program by access `Tape::config`.
