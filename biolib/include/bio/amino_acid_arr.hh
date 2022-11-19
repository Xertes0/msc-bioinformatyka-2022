/**
 * Amino acid array.
 * @file amino_acid_arr.hh
 */

#pragma once

#include <array>

namespace bio
{

/**
 * Returns bio::amino_acid_arr index corresponding to nucleotide's symbol.
 * @param symbol Nucleotide's symbol.
 * @throws Throws std::runtime_error when invalid amino acid character was passed.
 * @note Uracil and thymine are treated the same.
 * @see bio::amino_acid_arr
 */
std::size_t
ctoaaai(char symbol)
{
    switch(symbol) {
        case 'T': return 0;
        case 'U': return 0;
        case 'C': return 1;
        case 'A': return 2;
        case 'G': return 3;
        default: throw std::runtime_error{"Invalid amino acid char"};
    }
}

/**
 * Amino acid array.
 * See bio::ctoaaai on how to index.
 * @see bio::ctoaaai
 */
static
constexpr
std::array<std::array<std::array<char, 4>, 4>, 4> amino_acid_arr
{
    std::array<std::array<char,4>, 4>
    {
        std::array<char,4>
        {
            'F',
            'F',
            'L',
            'L',
        },
        {
            'S',
            'S',
            'S',
            'S',
        },
        {
            'Y',
            'Y',
            '-',
            '-',
        },
        {
            'C',
            'C',
            '-',
            'W',
        },
    },
    std::array<std::array<char,4>, 4>
    {
        std::array<char,4>
        {
            'L',
            'L',
            'L',
            'L',
        },
        {
            'P',
            'P',
            'P',
            'P',
        },
        {
            'H',
            'H',
            'Q',
            'Q',
        },
        {
            'R',
            'R',
            'R',
            'R',
        },
    },
    std::array<std::array<char,4>, 4>
    {
        std::array<char,4>
        {
            'I',
            'I',
            'I',
            'M',
        },
        {
            'T',
            'T',
            'T',
            'T',
        },
        {
            'N',
            'N',
            'K',
            'K',
        },
        {
            'S',
            'S',
            'R',
            'R',
        },
    },
    std::array<std::array<char,4>, 4>
    {
        std::array<char,4>
        {
            'V',
            'V',
            'V',
            'V',
        },
        {
            'A',
            'A',
            'A',
            'A',
        },
        {
            'D',
            'D',
            'E',
            'E',
        },
        {
            'G',
            'G',
            'G',
            'G',
        },
    },
};

static_assert(amino_acid_arr[0][0][0] == 'F');
static_assert(amino_acid_arr[0][1][0] == 'S');

} // namespace bio