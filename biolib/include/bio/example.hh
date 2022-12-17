/**
 * @file example.hh
 * Example file to test embind.
 */

#pragma once

namespace bio
{

/**
 * Test function to test embind.
 * @return @c int 10.
 */
int
test();

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_example)
{
    emscripten::function("test", &bio::test);
}

#endif
