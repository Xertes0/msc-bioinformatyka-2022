#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include <bio/skeletal/detail/format.hh>
#include <bio/skeletal/detail/radian.hh>

TEST_CASE("bio::skeletal::detail::to_radians", "[biolib][detail][to_radians]")
{
    using bio::skeletal::detail::to_radians;

    REQUIRE(to_radians(0) == 0.0);
    REQUIRE(to_radians(45) == Catch::Approx(std::numbers::pi/4));
    REQUIRE(to_radians(90) == Catch::Approx(std::numbers::pi/2));
    REQUIRE(to_radians(180) == Catch::Approx(std::numbers::pi));
    REQUIRE(to_radians(270) == Catch::Approx(3*std::numbers::pi/2));
    REQUIRE(to_radians(360) == Catch::Approx(2*std::numbers::pi));
}

TEST_CASE("bio::skeletal::detail::format", "[biolib][detail][format]")
{
    using bio::skeletal::detail::format;
    std::stringstream sstream{};
    sstream << '-' << "char_arr" << "char_ptr" << "string_view";

    std::string format_out{};
    char const* char_ptr = "char_ptr";
    std::string_view const string_view = "string_view";
    format(format_out, '-', "char_arr", char_ptr, string_view);

    REQUIRE(format_out == sstream.str());
}

TEST_CASE("bio::skeletal::detail::format double", "[biolib][detail][format]")
{
    using bio::skeletal::detail::format;

    std::string format_out{};
    static constexpr double NUMBER = 87423.123;
    format(format_out, NUMBER);
    REQUIRE(std::stod(format_out) == Catch::Approx(NUMBER));
}
