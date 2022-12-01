#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/text.hh"
#include "bio/skeletal/text_placement.hh"
#include "bio/skeletal/text_style.hh"

#include "bio/skeletal/detail/format.hh"

namespace bio::skeletal
{

template<char OneLetterId, bond_type FirstBond, class SideChainDesc>
struct
basic_amino_acid
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx)
    {
        detail::format(str, "<symbol id='aa_cache-", OneLetterId, ctx.flip?"f":"", "'>");

        if(ctx.flip) {
            basic_text<
            text_placement_right,
                    basic_text_span<
                    text_style_normal,
                    basic_text_rep<'N'>
                    >,
                    basic_text_span<
                    text_style_below,
                    basic_text_rep<'H'>
                    >
                    >::draw(str, ctx, ctx.flip);
        } else {
            basic_text<
            text_placement_right,
                    basic_text_span<
                    text_style_normal,
                    basic_text_rep<'N'>
                    >,
                    basic_text_span<
                    text_style_above,
                    basic_text_rep<'H'>
                    >
                    >::draw(str, ctx, ctx.flip);
        }

        fixed_offset<static_cast<int>(TEXT_MARGIN*2) + (SINGLE_CHAR_OFFSET_X*2), 0>::draw(str, ctx, ctx.flip);

        auto first_bond = FirstBond;
        if(ctx.flip) {
            if(first_bond == bond_type::wedged) {
                first_bond = bond_type::dashed;
            } else if(first_bond == bond_type::dashed) {
                first_bond = bond_type::wedged;
            }
        }

        draw_bond(str, ctx, first_bond, 80);
        auto side_chain = draw_context{ctx};
        draw_bond(str, ctx, bond_type::plain, 80);
        auto dbond = draw_context{ctx};
        draw_bond(str, ctx, bond_type::plain, 80);

        draw_bond(str, dbond, bond_type::double_bond, -45);
        basic_text<
        text_placement_down,
                basic_text_span<
                text_style_normal,
                basic_text_rep<'O'>
                >
                >::draw(str, dbond, !dbond.flip);
        SideChainDesc::draw(str, side_chain);

        detail::format(str, "</symbol>");
    }
};

} // namespace bio::skeletal
