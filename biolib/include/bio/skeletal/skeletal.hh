/**
 * @file skeletal.hh
 * Draw a skeletal formula of amino acids.
 */

#pragma once

#include <string>

namespace bio
{

/**
 * Draw skeletal formula from amino acid sequence
 *
 * Example:
 * @code
 * auto svg = bio::draw_skeletal("MKLISA");
 * @endcode
 *
 * @remark This function takes <tt>std::string const&</tt> instead of <tt>std::span<char></tt> because of embind does not support the later.
 *
 * @param sequence Amino acid sequence.
 * @return Valid @c svg file contents.
 */
[[nodiscard]]
std::string
//draw_skeletal(std::span<char> sequence);
draw_skeletal(std::string const& sequence);

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_skeletal_skeletal)
{
    emscripten::function("bio_draw_skeletal", &bio::draw_skeletal);
}

#endif
