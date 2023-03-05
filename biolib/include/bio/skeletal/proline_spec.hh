#pragma once

#include <string>

#include <gcem.hpp>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

struct proline_circle
{
    static
    constexpr
    void draw(std::string& str, draw_context& ctx, bool flip)
    {
        int angle = 125;

        constexpr double A3 = 25.0;
        constexpr double A = A3 / gcem::sqrt(3);

        //bool ctxflip = ctx.flip;
        //draw_bond<static_cast<int>(A)>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);
        constexpr double S1 = A / gcem::sqrt(2);

        bool ctxflip = ctx.flip;
        angle -= 45;
        draw_bond<static_cast<int>(S1)>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);

        ctx.flip = ctxflip;
        angle += 90;
        draw_bond<static_cast<int>(S1)>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);

        //ctx.flip = ctxflip;
        //angle += 180-60;
        //draw_bond<A*2>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);
        constexpr double S2 = (A*2.0) / gcem::sqrt(2);

        ctx.flip = ctxflip;
        angle += (180-60) - (45*2);
        draw_bond<static_cast<int>(S2)>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);

        ctx.flip = ctxflip;
        angle += 90;
        draw_bond<static_cast<int>(S2)>(str, ctx, bond_type::plain, flip?angle:(360 - angle), 0, true);
    }
};

} // namespace bio::skeletal
