#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <iterator>
#include <span>
#include <sstream>

#include <gcem.hpp>

#include "bio/skeletal/cache.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/skeletal.hh"
#include "bio/skeletal/svg_defs.hh"

namespace bio::skeletal
{

void
draw(std::stringstream& sstream, draw_context& ctx, char one_letter_id)
{
    sstream <<
        "<use href='#aa_cache-" <<
        one_letter_id <<
        (ctx.flip?"f":"") <<
        "' x='" <<
        ctx.x - DRAW_CTX_OFFSET_X + (TEXT_MARGIN*2) <<
        "' y='" <<
        ctx.y - DRAW_CTX_OFFSET_Y <<
        "'></use>";

    ctx.x += (gcem::cos(detail::to_radians(-45+80)) * BOND_LENGTH * 3) + (SINGLE_CHAR_OFFSET_X * 2) + (TEXT_MARGIN*2); // single char offset appears to be a half char offset
    ctx.y += (gcem::sin(detail::to_radians(-45+80)) * BOND_LENGTH) * (ctx.flip?-1:1);
    ctx.flip = !ctx.flip;
}

void
draw_end(std::string& str, draw_context& ctx)
{
    fixed_offset<static_cast<int>(TEXT_MARGIN*2), 0>::draw(str, ctx, false);
    basic_text<
        text_placement_right,
        basic_text_span<
            text_style_normal,
            basic_text_rep<'O'>
        >,
        basic_text_span<
            text_style_high_nm,
            basic_text_rep<'-'>
        >
    >::draw(str, ctx, false);
}

} // namespace bio::skeletal

namespace bio
{

std::string
draw_skeletal(std::string const& sequence)
{
    auto [header_arr, header_length] = bio::skeletal::cache_header();

    std::stringstream sstream{};
    sstream
        << "<svg viewBox='60.25 25.08 "
        << (bio::skeletal::SVG_AA_WIDTH*static_cast<double>(sequence.length()))+bio::skeletal::SVG_SIDE_WIDTH
        << ' '
        << bio::skeletal::SVG_MAX_HEIGHT
        << "'>";
    std::copy(header_arr.begin(), header_arr.begin()+header_length, std::ostream_iterator<char>(sstream));

    bio::skeletal::draw_context ctx{};

    for(char aa_id : sequence) {
        draw(sstream, ctx, aa_id);
    }

    auto str = sstream.str();
    bio::skeletal::draw_end(str, ctx);

    str += "</svg>";

    return str;
}

} // namespace bio

#pragma clang diagnostic pop
