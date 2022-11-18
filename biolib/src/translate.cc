#include "bio/translate.hh"

#include <range/v3/all.hpp>

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
            return amino_acid_arr[*(beg++)][*(beg++)][*beg];
        })
        | ranges::to<std::string>();
}

} // namespace bio