#pragma once

#include <array>
#include <string>
#include <vector>

namespace bio
{

[[nodiscard]]
std::array<std::string, 3>
translate(std::string const& sequence);

} // namespace bio