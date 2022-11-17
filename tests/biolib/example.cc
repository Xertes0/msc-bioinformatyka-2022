#include <catch2/catch_test_macros.hpp>

#include <bio/example.hh>

TEST_CASE("biolib", "test")
{
    REQUIRE(bio::test() == 75);
}