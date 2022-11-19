#include "bio/translate.hh"

#include <cassert>
#include <cctype>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "bio/amino_acid_arr.hh"

namespace bio
{

std::array<std::string, 3>
translate(std::string const& sequence)
{
    auto const orf = [](auto range){
        return range
            // Chunk by 3 nucleotides
            | ranges::views::chunk(3)
            // Last group can be < 3 so filter it out
            | ranges::views::filter([](auto group) { return ranges::distance(group)==3; })
            // transform chars to indexes
            | ranges::views::transform([](auto group) { return group | ranges::views::transform([](auto nucl){ return ctoaaai(nucl); }); })
            // return amino acid char representation
            | ranges::views::transform([](auto group) {
                auto beg = group.begin();
                assert(ranges::distance(group) == 3);
#ifndef NDEBUG
                return amino_acid_arr.at(*(beg++)).at(*(beg++)).at(*beg);
#else
                return amino_acid_arr[*(beg++)][*(beg++)][*beg]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
#endif
            })
            | ranges::to<std::string>();
    };

    auto prepared = sequence
        // Filter out whitespaces
        | ranges::views::filter([](auto nucl) { return std::isspace(nucl) == 0; });

    return std::array<std::string, 3> {
        orf(prepared),
        orf(prepared | ranges::views::drop(1)),
        orf(prepared | ranges::views::drop(2))
    };
}

} // namespace bio