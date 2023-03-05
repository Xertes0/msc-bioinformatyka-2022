#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/text.hh"
#include "bio/skeletal/text_placement.hh"
#include "bio/skeletal/text_style.hh"

namespace bio::skeletal
{

struct tryptophan_circle
{
    static
    constexpr
    void draw(std::string& str, draw_context& ctx, bool flip)
    {
        for(int i=0;i<5;++i) {
            int angle_a = ((360/5) * (i + 1)) - 90; //72.5;
            int angle_b = ((360/5) * (i + 1)) - 90; //72.5;

            enum bond_type bond = bond_type::plain;
            if(i == 0) {
                bond = bond_type::double_bond;
            }

            if(flip) {
                draw_bond<20>(str, ctx, bond, angle_a, angle_b, true);
            } else {
                draw_bond<20>(str, ctx, bond, 360 - angle_a, 360 - angle_b, true);
            }

            if(i == 1) {
                draw_context bctx{ctx};
                if(flip) {
                    basic_text<
                    text_placement_down_right,
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
                    text_placement_down_right,
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
            } else if(i == 2) {
                for(int j=0;j<6;++j) {
                    int angle_a6 = (((360/5) * (i + 2)) - 90) - (j * (360/6));
                    int angle_b6 = (((360/5) * (i + 2)) - 90) - (j * (360/6));

                    enum bond_type bond6 = bond_type::plain;
                    if(j%2!=0) {
                        bond6 = bond_type::double_bond;
                    }

                    if(flip) {
                        draw_bond<20>(str, ctx, bond6, angle_a6, angle_b6, true);
                    } else {
                        draw_bond<20>(str, ctx, bond6, 360 - angle_a6, 360 - angle_b6, true);
                    }
                }
            }
        }
    }
};

} // namespace bio::skeletal
