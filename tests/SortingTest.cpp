#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <filesystem>
#include <fstream>
#include <functional>
#include <random>

#include "../src/Tape/Tape.h"
#include "../src/Tape/Sort.h"

TEST_CASE( "Tape sort testing", "[Tape sort]")
{
    std::filesystem::remove_all("tmp");
    SECTION( "The worst sequence" )
    {
        int items[1000];
        for (int i = 0; i < 1000; ++ i)
            items[i] = i;
        std::ofstream ostr ("intape");
        ostr.write(reinterpret_cast<char*>(&items), 1000 * sizeof(int));
        ostr.close();

        Tape::Tape<int> intape("intape");
        Tape::Tape<int> outtape = Tape::CreateTape<int>("outtape", intape.getSize());
        Tape::NaturalSorting(intape, outtape, std::greater<int>());

        std::ifstream istr ("outtape");
        istr.read(reinterpret_cast<char*>(&items), 1000 * sizeof(int));
        istr.close();

        REQUIRE(std::is_sorted(items, items + 1000, std::greater<int>()));
    }
    SECTION( "Random sequence" )
    {
        std::default_random_engine rnd(std::time(nullptr));
        std::uniform_int_distribution<int> dstr (0, 100000);

        int items[10000];
        for (int i = 0; i < 10000; ++ i)
            items[i] = dstr(rnd);
        std::ofstream ostr ("intape");
        ostr.write(reinterpret_cast<char*>(&items), 10000 * sizeof(int));
        ostr.close();

        Tape::Tape<int> intape("intape");
        Tape::Tape<int> outtape = Tape::CreateTape<int>("outtape", intape.getSize());
        Tape::NaturalSorting(intape, outtape, std::greater<int>());

        std::ifstream istr ("outtape");
        istr.read(reinterpret_cast<char*>(&items), 10000 * sizeof(int));
        istr.close();

        REQUIRE(std::is_sorted(items, items + 10000, std::greater<int>()));
    }
    /*
    SECTION( "Big sequence sorting" )
    {
        std::default_random_engine rnd(std::time(nullptr));
        std::uniform_int_distribution<int> dstr (0, 1000000);

        int items[1000000];
        for (int i = 0; i < 1000000; ++ i)
            items[i] = dstr(rnd);
        std::ofstream ostr ("intape");
        ostr.write(reinterpret_cast<char*>(&items), 1000000 * sizeof(int));
        ostr.close();

        Tape::Tape<int> intape("intape");
        Tape::Tape<int> outtape = Tape::CreateTape<int>("outtape", intape.getSize());
        Tape::NaturalSorting(intape, outtape, std::greater<int>());

        std::ifstream istr ("outtape");
        istr.read(reinterpret_cast<char*>(&items), 1000000 * sizeof(int));
        istr.close();

        REQUIRE(std::is_sorted(items, items + 1000000, std::greater<int>()));
    }
    */
}
