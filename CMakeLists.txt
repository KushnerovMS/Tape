cmake_minimum_required(VERSION 3.16) 

project(Tape)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE Tape_src
    src/Tape/*.h src/Tape/*.cpp)

add_executable(TapeSort src/main.cpp  ${Tape_src})

add_subdirectory(tests)
