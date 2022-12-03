#pragma once

#include <array>
#include <functional>
#include <string>
#include <tuple>
#include <vector>

#ifdef EMSCRIPTEN

#include <emscripten/val.h>

#endif

namespace bio
{

[[nodiscard]]
std::array<std::string, 3>
#ifdef EMSCRIPTEN
translate(std::string const& sequence, emscripten::val surround_cb);
#else
translate(std::string const& sequence, std::function<std::tuple<std::string, std::string>(std::size_t)> surround_cb);
#endif

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_translatet)
{
    emscripten::value_array<std::array<std::string, 2>>("array_string_2")
            .element(emscripten::index<0>())
            .element(emscripten::index<1>());

    emscripten::value_array<std::array<std::string, 3>>("array_string_3")
            .element(emscripten::index<0>())
            .element(emscripten::index<1>())
            .element(emscripten::index<2>());

    emscripten::function("bio_translate", &bio::translate);
}

#endif