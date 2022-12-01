#pragma once

namespace bio
{

int
test();

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_example)
{
    emscripten::function("bio_test", &bio::test);
}

#endif