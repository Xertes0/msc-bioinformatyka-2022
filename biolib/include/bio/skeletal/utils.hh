#pragma once

#include <string>

#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

template<class... Args>
struct back_after
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        draw_context bctx{ctx};
        (..., Args::draw(str, bctx, flip));
    }
};

} // namespace bio::skeletal
