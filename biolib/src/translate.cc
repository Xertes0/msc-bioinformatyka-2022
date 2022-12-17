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

std::array<std::string, 3>
#ifdef EMSCRIPTEN
translate(std::string const& sequence, emscripten::val surround_cb)
#else
translate(std::string const& sequence, std::function<std::tuple<std::string, std::string>(std::size_t)> surround_cb)
#endif
{
    auto const orf = [&surround_cb](auto range){
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

        std::string str{};
        std::string close_str{};
        str.reserve(new_range.size());
        bool open = false;
        std::size_t index = 0;
        for(char const amino_acid : new_range) {
            if(amino_acid == 'M') {
                if(!open) {
                    struct helper
                    {
                        std::string& to_insert;

                        helper&
                        operator=(std::string const& str)
                        {
                            to_insert.append(str);
                            return *this;
                        }
                    } helper{str};
                    std::tie(helper, close_str) = surround_cb(index++)
                    #ifdef EMSCRIPTEN
                            .as<std::array<std::string, 2>>()
                    #endif
                    ;
                    str.push_back(amino_acid);
                    open = true;
                } else {
                    str.push_back(amino_acid);
                }
            } else if(amino_acid == '-') {
                if(open) {
                    str.append(close_str);
                    open = false;
                }
                str.push_back('-');
            } else {
                str.push_back(amino_acid);
            }
        }

        if(open) {
            str.append(close_str);
        }

        return str;
    };

    return std::array<std::string, 3> {
        orf(sequence),
        orf(sequence | ranges::views::drop(1)),
        orf(sequence | ranges::views::drop(2))
    };
}

} // namespace bio
