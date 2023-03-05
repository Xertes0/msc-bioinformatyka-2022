#pragma once

#include <string>

#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/bond.hh"

namespace bio::skeletal
{

template<class... Args>
struct
basic_side_chain
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx)
    {
        [[maybe_unused]] auto flip = ctx.flip;
        ctx.flip = false;
        (Args::draw(str, ctx, flip), ...);
    }
};

template<bond_type BondType, int AngleA = 90, int AngleB = 35>
struct
basic_side_chain_bond
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        auto bond = BondType;
        if(flip) {
            if(bond == bond_type::wedged) {
                bond = bond_type::dashed;
            } else if(bond == bond_type::dashed) {
                bond = bond_type::wedged;
            }
        }

        if(flip) {
            draw_bond(str, ctx, bond, AngleA, AngleB, true);
        } else {
            draw_bond(str, ctx, bond, 360 - AngleA, 360 - AngleB, true);
        }
    }
};

} // namespace bio::skeletal
