#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <cmath>
#include <iterator>
#include <span>
#include <sstream>

#include "bio/skeletal/cache.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/skeletal.hh"

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
        "' />";

    ctx.x += (std::cos(detail::to_radians(-45+80)) * BOND_LENGTH * 3) + (SINGLE_CHAR_OFFSET_X * 2) + (TEXT_MARGIN*2); // single char offset appears to be a half char offset
    ctx.y += (std::sin(detail::to_radians(-45+80)) * BOND_LENGTH) * (ctx.flip?-1:1);
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

    str.append("</svg>\n");
}

} // namespace bio::skeletal

namespace bio
{

std::string
draw_skeletal(std::span<char> sequence)
{
    auto [header_arr, header_length] = bio::skeletal::cache_header();

    std::stringstream sstream{};
    std::copy(header_arr.begin(), header_arr.begin()+header_length, std::ostream_iterator<char>(sstream));

    bio::skeletal::draw_context ctx{};

    for(char aa_id : sequence) {
        draw(sstream, ctx, aa_id);
    }

    auto str = sstream.str();
    bio::skeletal::draw_end(str, ctx);

    return str;
}

} // namespace bio

#pragma clang diagnostic pop