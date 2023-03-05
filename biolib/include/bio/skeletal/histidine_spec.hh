#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/text_placement.hh"
#include "bio/skeletal/text_style.hh"

namespace bio::skeletal
{

struct histidine_circle
{
    static
    constexpr
    void draw(std::string& str, draw_context& ctx, bool flip)
    {
        for(int i=0;i<5;++i) {
            int angle_a = ((360/5) * (i + 1)) - 90; //72.5;
            int angle_b = ((360/5) * (i + 1)) - 90; //72.5;

            enum bond_type bond = bond_type::plain;
            if(i%3==1) {
                bond = bond_type::double_bond;
            }

            if(flip) {
                draw_bond<20>(str, ctx, bond, angle_a, angle_b, true);
            } else {
                draw_bond<20>(str, ctx, bond, 360 - angle_a, 360 - angle_b, true);
            }

            if(i == 0) {
                draw_context bctx{ctx};
                basic_text<
                    text_placement_up_right,
                    basic_text_span<
                        text_style_normal,
                        basic_text_rep<'N'>
                    >
                >::draw(str, bctx, flip);
            } else if(i == 2) {
                draw_context bctx{ctx};
                if(flip) {
                    basic_text<
                    text_placement_down,
                        basic_text_span<
                            text_style_normal,
                            basic_text_rep<'N'>
                        >,
                        basic_text_span<
                            text_style_below,
                            basic_text_rep<'H'>
                        >
                        >::draw(str, bctx, flip);
                } else {
                    basic_text<
                    text_placement_down,
                        basic_text_span<
                            text_style_normal,
                            basic_text_rep<'N'>
                        >,
                        basic_text_span<
                            text_style_above,
                            basic_text_rep<'H'>
                        >
                        >::draw(str, bctx, flip);
                }
            }
        }
    }
};

} // namespace bio::skeletal
