find_package(Catch2 3 REQUIRED)

enable_testing ()

add_executable (TapeTest TapeTest.cpp ${Tape_src})
target_link_libraries(TapeTest PRIVATE Catch2::Catch2WithMain)

add_test (NAME TapeTest COMMAND TapeTest)
