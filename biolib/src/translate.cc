#include "bio/translate.hh"

#include <cassert>
#include <cctype>
#include <tuple>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "bio/amino_acid_arr.hh"

namespace bio
{

std::array<bio::orf_result, 3>
translate(std::string const& sequence)
{
    auto const orf = [](auto range){
        auto new_range = range
            // To upper case
            | ranges::views::transform([](auto const nucl){ return std::toupper(nucl); })
            // Filter out invalid characters
            | ranges::views::filter([](auto const nucl) {
                return
                    nucl == 'T' ||
                    nucl == 'U' ||
                    nucl == 'C' ||
                    nucl == 'A' ||
                    nucl == 'G';
            })
            // Chunk by 3 nucleotides
            | ranges::views::chunk(3)
            // Last group can be < 3 so filter it out
            | ranges::views::filter([](auto group) { return ranges::distance(group)==3; })
            // transform chars to indexes
            | ranges::views::transform([](auto group) { return group | ranges::views::transform(ctoaaai); })
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

        bio::orf_result res{};
        res.sequence = std::move(new_range);
        for(std::size_t str_i=0;str_i<res.sequence.length();++str_i) {
            if(res.sequence[str_i] == 'M') {
                for(std::size_t find_i=str_i+1;find_i<res.sequence.length();++find_i) {
                    if(res.sequence[find_i] == '-') {
                        res.indices.emplace_back(std::pair{str_i, find_i});
                        break;
                    }
                }
                // ignore peptides without terminator
            }
        }

        return res;
    };

    return std::array<bio::orf_result, 3> {
        orf(sequence),
        orf(sequence | ranges::views::drop(1)),
        orf(sequence | ranges::views::drop(2))
    };
}

} // namespace bio
