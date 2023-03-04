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
 * Result of one translating one ORF.
 *
 * @see bio::translate for example usage
 */
struct orf_result
{
    /**
     * Translated sequence.
     */
    std::string sequence;

    /**
     * Vector of pairs specifying the start and end indexes of peptides.
     */
    std::vector<std::pair<std::size_t, std::size_t>> indices;
};

/**
 * Translate a DNA/RNA sequence into 3 ORFs.
 *
 * Example:
 * @code
 *  auto orfs = bio::translate(
 *      std::string{"AGCAGCAGAGAUGACGAGCAGAGCGAGCUGAAAGAAAAUGGAGCGCUGA"}
 *  );
 *
 *  assert(orfs[0].sequence == "SSRDDEQSELKENGAL");
 *  assert(orfs[0].indices.empty());
 *
 *  assert(orfs[1].sequence == "AAEMTSRAS-KKMER-");
 *  assert(orfs[1].indices.size() == 2);
 *
 *  assert(orfs[1].indices[0].first == 3);
 *  assert(orfs[1].indices[0].second == 9);
 *
 *  assert(orfs[1].indices[1].first == 12);
 *  assert(orfs[1].indices[1].second == 15);
 *
 *  assert(orfs[2].sequence == "QQR-RAERAERKWSA");
 *  assert(orfs[2].indices.empty());
 * @endcode
 *
 * @remark This function takes <tt>std::string const&</tt> instead of <tt>std::string_view</tt> because embind dosen't support the later.
 *
 * @param sequence DNA/RNA sequence.
 * @return Array of 3 bio::orf_result.
 *
 * @see bio::orf_result
 */
[[nodiscard]]
std::array<bio::orf_result, 3>
#ifdef EMSCRIPTEN
translate(std::string const& sequence);
#else
translate(std::string const& sequence);
#endif

} // namespace bio

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(bio_translate)
{
    emscripten::class_<bio::orf_result>("orf_result")
        .property("sequence", &bio::orf_result::sequence)
        .property("indices", &bio::orf_result::indices);

    emscripten::value_array<std::array<bio::orf_result, 3>>("array-orf_result-3")
            .element(emscripten::index<0>())
            .element(emscripten::index<1>())
            .element(emscripten::index<2>());

    emscripten::function("translate", &bio::translate);
}

#endif
