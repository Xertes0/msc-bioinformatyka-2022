#pragma once

#include <numbers>

namespace bio::skeletal::detail
{

[[nodiscard]]
constexpr
double
to_radians(double degrees)
{
    return (degrees / 180.0) * std::numbers::pi_v<double>; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
}

} // namespace bio::skeletal::detail