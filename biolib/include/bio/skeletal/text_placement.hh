#pragma once

#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/text.hh"
#include "bio/skeletal/detail/format.hh"

namespace bio::skeletal
{

template<bool Right>
struct
basic_text_placement_straight
{
    static
    constexpr
    void
    append(std::string &str, draw_context &ctx, bool flip)
    {
        detail::format(str,
                       "text-anchor='",
                       (Right ? "start" : "middle"),
                       "' dominant-baseline='",
                       (Right ? "mathematical" : (flip ? "hanging" : "text-top")),
                       "' x='", ctx.x + (Right ? TEXT_MARGIN : 0),
                       "' y='", ctx.y + ((Right ? 0 : TEXT_MARGIN) * (flip ? 1 : -1)),
                       "'");
    }
};

template<bool Right>
struct
basic_text_placement_angle
{
    static
    constexpr
    void
    append(std::string &str, draw_context &ctx, bool flip)
    {
        detail::format(str,
                       "text-anchor='",
                       (Right ? "start" : "end"),
                       "' dominant-baseline='",
                       (flip ? "hanging" : "text-top"),
                       "' x='", ctx.x + (Right ? TEXT_MARGIN : 0),
                       "' y='", ctx.y + ((Right ? 0 : TEXT_MARGIN) * (flip ? 1 : -1)),
                       "'");
    }
};

using text_placement_right = basic_text_placement_straight<true>;
using text_placement_down  = basic_text_placement_straight<false>;
using text_placement_down_right = basic_text_placement_angle<true>;
using text_placement_down_left  = basic_text_placement_angle<false>;

} // namespace bio::skeletal