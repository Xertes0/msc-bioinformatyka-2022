#pragma once

#include <string>

#include "bio/skeletal/detail/format.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

struct arqinine_circle
{
    static
    constexpr
    void draw(std::string& str, draw_context& ctx, [[maybe_unused]] bool flip = false)
    {
        detail::format(str, "<circle cx=\"", ctx.x, "\" cy=\"", ctx.y, "\" r=\"6\" fill=\"white\" stroke=\"black\" />");
        detail::format(str, "<text text-anchor=\"middle\" dominant-baseline=\"middle\" x=\"", ctx.x, "\" y=\"", ctx.y, "\">+</text>");
    }
};

} // namespace bio::skeletal
