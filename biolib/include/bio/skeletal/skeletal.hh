#pragma once

#include <string>

namespace bio
{

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
