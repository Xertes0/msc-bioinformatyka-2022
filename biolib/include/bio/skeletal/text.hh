#pragma once

#include <string>

#include "bio/skeletal/detail/format.hh"
#include "bio/skeletal/detail/radian.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

static constexpr double TEXT_MARGIN = 2;

static constexpr int SINGLE_CHAR_OFFSET_X = 4;
static constexpr int SINGLE_CHAR_OFFSET_Y = 8;

template<char... Args>
struct
basic_text_rep
{
    static
    constexpr
    void
    append(std::string &str)
    {
        (str.push_back(Args), ...);
    }
};

struct
noop
{
    static
    constexpr
    std::string_view value{};
};

template<class TextStyle, class TextRep, class Aditional = noop>
struct
basic_text_span
{
    static
    constexpr
    void
    draw(std::string &str)
    {
        detail::format(str, "<tspan ", TextStyle::value, " ", Aditional::value, ">");
        TextRep::append(str);
        detail::format(str, "</tspan>");
    }
};

template<class TextPlacement, class... Content>
struct
basic_text
{
    static
    constexpr
    void
    draw(std::string &str, draw_context &ctx, bool flip)
    {
        detail::format(str, "<text ");
        TextPlacement::append(str, ctx, flip);
        detail::format(str, ">");
        (Content::draw(str), ...);
        detail::format(str, "</text>");
    }
};

template<int OffsetX, int OffsetY>//, class Suffix = text_rep<'p', 'x'>>
struct
fixed_offset
{
    static
    constexpr
    void
    draw([[maybe_unused]] std::string &str, draw_context &ctx, bool flip)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (flip ? 1 : -1);
    }
};

} // namespace bio::skeletal