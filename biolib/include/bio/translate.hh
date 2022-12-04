/**
 * @file translate.hh
 * Translating DNA/RNA sequence.
 */

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

/**
 * Translate a DNA/RNA sequence into 3 ORFs.
 *
 * Example:
 * @code
 *  auto orfs = bio::translate(
 *          std::string{"AGCAGCAGAGAUGACGAGCAGAGCGAGCUGAAAGAAAAUGGAGCGCUGA"},
 *          [](std::size_t const index){
 *              return std::tuple{std::string{"b"} + std::to_string(index), std::to_string(index) + "e"};
 *          }
 *  );
 *
 *  assert(orfs[0] == "SSRDDEQSELKENGAL");
 *  assert(orfs[1] == "AAEb0MTSRAS0e-KKb1MER1e-");
 *  assert(orfs[2] == "QQR-RAERAERKWSA");
 * @endcode
 *
 * @remark This function takes <tt>std::string const&</tt> instead of <tt>std::string_view</tt> because embind dosen't support the later.
 *
 * @param sequence DNA/RNA sequence.
 * @param surround_cb Function returning a tuple <i>(or array when using emscripten)</i> of two strings which will surround a peptide sequence.
 * @return Array of 3 ORFs.
 */
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