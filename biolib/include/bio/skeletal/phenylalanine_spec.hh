#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

struct phenylalanine_circle
{
    static
    constexpr
    void draw(std::string& str, draw_context& ctx, bool flip)
    {
        for(int i=0;i<6;++i) {
            int angle_a = ((360/6) * (i + 1)) - 90;
            int angle_b = ((360/6) * (i + 1)) - 90;

            enum bond_type bond = bond_type::plain;
            if(i%2!=0) {
                bond = bond_type::double_bond;
            }

            if(flip) {
                draw_bond<20>(str, ctx, bond, angle_a, angle_b, true);
            } else {
                draw_bond<20>(str, ctx, bond, 360 - angle_a, 360 - angle_b, true);
            }
        }
    }
};

} // namespace bio::skeletal
