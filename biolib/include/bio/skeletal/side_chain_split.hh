#pragma once

#include <string>

#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

template<class A, class B>
struct
basic_side_chain_split
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        ctx.flip = flip;
        draw_context bctx{ctx};
        A::draw(str, ctx);
        B::draw(str, bctx);
    }
};

struct
flip_flip
{
    static
    constexpr
    void
    draw([[maybe_unused]] std::string& str, draw_context& ctx, [[maybe_unused]] bool flip = false)
    {
        ctx.flip = !ctx.flip;
    }
};

template<bond_type BondType>
using basic_side_chain_split_bond_soft_a = basic_side_chain_bond<BondType, 135>;

template<bond_type BondType>
using basic_side_chain_split_bond_soft_b = basic_side_chain_bond<BondType, 45>;

template<bond_type BondType>
using basic_side_chain_split_bond_hard_a = basic_side_chain_bond<BondType, 90>;

template<bond_type BondType>
using basic_side_chain_split_bond_hard_b = basic_side_chain_bond<BondType, 0>;

} // namespace bio::skeletal
