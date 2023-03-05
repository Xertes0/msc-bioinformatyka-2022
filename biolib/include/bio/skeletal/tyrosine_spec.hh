#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/side_chain.hh"
#include "bio/skeletal/text.hh"
#include "bio/skeletal/text_placement.hh"
#include "bio/skeletal/text_style.hh"

namespace bio::skeletal
{

struct tyrosine_circle
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

            if(i == 2) {
                draw_context bctx{ctx};
                basic_side_chain_bond<bond_type::plain>::draw(str, bctx, flip);
                basic_text<
                    text_placement_down_right,
                    basic_text_span<
                        text_style_normal,
                        basic_text_rep<'O', 'H'>
                    >
                >::draw(str, bctx, flip);
            }
        }
    }
};

} // namespace bio::skeletal
