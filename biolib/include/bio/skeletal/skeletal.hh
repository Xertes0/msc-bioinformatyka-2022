#pragma once

#include <string>
#include <span>

namespace bio
{

//[[nodiscard]]
//std::string
//draw_skeletal(std::span<char> sequence);

[[nodiscard]]
std::string
draw_skeletal(std::string sequence);

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_skeletal_skeletal)
{
    emscripten::function("bio_draw_skeletal", &bio::draw_skeletal);
}

#endif
