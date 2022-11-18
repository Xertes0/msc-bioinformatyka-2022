#include "bio/translate.hh"

#include <cassert>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "bio/amino_acid_arr.hh"

namespace bio
{

std::string
translate(std::string const& sequence)
{
    return sequence
        | ranges::views::chunk(3)
        | ranges::views::filter([](auto group) { return group.size()==3 && *(group.end()-1) != '\0'; })
        | ranges::views::transform([](auto group) { return group | ranges::views::transform([](auto nucl){ return ctoaaai(nucl); }); })
        | ranges::views::transform([](auto group) {
            auto beg = group.begin();
            assert(group.size() == 3);
#ifndef NDEBUG
            return amino_acid_arr.at(*(beg++)).at(*(beg++)).at(*beg);
#else
            return amino_acid_arr[*(beg++)][*(beg++)][*beg]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
#endif
        })
        | ranges::to<std::string>();
}

} // namespace bio