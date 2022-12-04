/**
 * @file cache.hh
 * Cache svg header with amino acids.
 */

#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <tuple>

#include "bio/skeletal/defs.hh"

namespace bio::skeletal
{

/**
 * Size of the buffer returned by <tt>bio::skeletal::cache_header()</tt>.
 */
static constexpr std::size_t BUFFER_SIZE = 1024*50;

/**
 * Buffer returned by <tt>bio::skeletal::cache_header()</tt>.
 */
using buffer_t = std::array<char, BUFFER_SIZE>;

/**
 * Returns compile time evaluated svg header with amino acids.
 *
 * @attention The buffer does not contain the <tt><svg></tt> tag.
 *
 * @return Tuple with cached buffer, amount of used data.
 *
 * @see <tt>bio::skeletal::buffer_t</tt>
 */
[[nodiscard]]
consteval
std::tuple<buffer_t, std::size_t>
cache_header()
{
    std::string str{};

    str.append("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>");
    str.append("<defs><pattern id='bond-dashed' height='10%' width='10%'><line x1='0' y1='0' x2='10' y2='0' stroke-width='1' stroke='black'></line></pattern></defs>");

    auto append = [&]<class AminoAcid>([[maybe_unused]] AminoAcid) consteval {
        draw_context ctx{};
        AminoAcid::draw(str, ctx);

        ctx = draw_context{};
        ctx.flip = true;
        AminoAcid::draw(str, ctx);
    };

    append(defs::alanine{});
    append(defs::aspariqine{});
    append(defs::aspartate{});
    append(defs::cysteine{});
    append(defs::glycine{});
    append(defs::glutamate{});
    append(defs::glutamine{});
    append(defs::isoleucine{});
    append(defs::leucine{});
    append(defs::lysine{});
    append(defs::methionine{});
    append(defs::serine{});
    append(defs::threonine{});
    append(defs::valine{});

    draw_context ctx{};
    ctx.x -= SINGLE_CHAR_OFFSET_X*3.2;

    basic_text<
    text_placement_right,
            basic_text_span<
            text_style_normal,
            basic_text_rep<'H'>
            >,
            basic_text_span<
            text_style_small,
            basic_text_rep<'2'>
            >
            >::draw(str, ctx, false);

    fixed_offset<SINGLE_CHAR_OFFSET_X*2, SINGLE_CHAR_OFFSET_Y>::draw(str, ctx, false);
    basic_text<
    text_placement_right,
            basic_text_span<
            text_style_normal,
            basic_text_rep<'+'>
            >
            >::draw(str, ctx, false);

    buffer_t buf{};
    std::copy(str.begin(), str.end(), buf.data());

    return {buf, str.length()};
}

} // namespace bio::skeletal