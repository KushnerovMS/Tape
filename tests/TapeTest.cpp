#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <filesystem>
#include <fstream>
#include "../src/Tape/Tape.h"

TEST_CASE( "Tape testing", "[Tape]")
{
    SECTION( "No tape file test" )
    {
        std::filesystem::remove("tape");
        REQUIRE_THROWS_WITH(Tape::Tape<int>("tape"),
                            Catch::Matchers::ContainsSubstring("No file")); 
    }
    SECTION( "Creating tape file test" )
    {
        REQUIRE_NOTHROW(Tape::CreateTape<int>("tape", 1));
        REQUIRE_NOTHROW(Tape::Tape<int>("tape"));
    }
    SECTION( "Created tape size check" )
    {
        REQUIRE(Tape::CreateTape<int>("tape", 100).getSize() == 100);
    }

    SECTION( "Read/write test" )
    {
        SECTION( "Reading" )
        {
            int item = 3;
            std::ofstream str ("tape");
            str.write(reinterpret_cast<char*>(&item), sizeof(int));
            str.close();
            REQUIRE( Tape::Tape<int>("tape").getItem() == 3);
        }

        SECTION( "Writing" )
        {
            REQUIRE_NOTHROW(Tape::Tape<int>("tape").setItem(5));
            int item = 0;
            std::ifstream str ("tape");
            str.read(reinterpret_cast<char*>(&item), sizeof(int));
            str.close();
            REQUIRE( item == 5);
        }
    }

    SECTION( "Moving" )
    {
        int items[] = {0, 1, 2};
        std::ofstream str ("tape");
        str.write(reinterpret_cast<char*>(&items), 3 * sizeof(int));
        str.close();
        Tape::Tape<int> tape("tape");

        SECTION( "Moving to sides" )
        {
            tape.toEnd();
            REQUIRE(tape.getItem() == 2);
            tape.toBeg();
            REQUIRE(tape.getItem() == 0);
        }
        SECTION( "Moving forward" )
        {
            REQUIRE(tape.Next());
            REQUIRE(tape.getItem() == 1);
            REQUIRE(tape.Next());
            REQUIRE(tape.getItem() == 2);
            REQUIRE_FALSE(tape.Next());
            REQUIRE_FALSE(tape.Next());
        }
        SECTION( "Moving backward" )
        {
            tape.toEnd();
            REQUIRE(tape.Prev());
            REQUIRE(tape.getItem() == 1);
            REQUIRE(tape.Prev());
            REQUIRE(tape.getItem() == 0);
            REQUIRE_FALSE(tape.Prev());
            REQUIRE_FALSE(tape.Prev());
        }
        SECTION( "At begin/end check" )
        {
            tape.toBeg();
            REQUIRE(tape.atBeg());
            REQUIRE_FALSE(tape.atEnd());
            tape.Next();
            REQUIRE_FALSE(tape.atBeg());
            REQUIRE_FALSE(tape.atEnd());
            tape.Next();
            REQUIRE_FALSE(tape.atBeg());
            REQUIRE(tape.atEnd());
        }
    }
}
