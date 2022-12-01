#pragma once

#include "bio/skeletal/amino_acid.hh"
#include "bio/skeletal/bond.hh"
#include "bio/skeletal/draw_context.hh"
#include "bio/skeletal/side_chain.hh"
#include "bio/skeletal/side_chain_split.hh"
#include "bio/skeletal/text.hh"
#include "bio/skeletal/text_placement.hh"
#include "bio/skeletal/text_style.hh"

namespace bio::skeletal::defs
{

using alanine =
        basic_amino_acid<
                'A',
                bond_type::plain,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::wedged>
                >
        >;

using aspariqine =
        basic_amino_acid<
                'N',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_a<bond_type::double_bond>,
                                        basic_text<
                                                text_placement_down,
                                                basic_text_span<text_style_normal, basic_text_rep<'O'>>
                                        >
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_b<bond_type::plain>,
                                        basic_text<
                                                text_placement_right,
                                                basic_text_span<text_style_normal, basic_text_rep<'N', 'H'>>,
                                                basic_text_span<text_style_small, basic_text_rep<'2'>>
                                        >
                                >
                        >
                >
        >;

using aspartate =
        basic_amino_acid<
                'D',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_a<bond_type::double_bond>,
                                        basic_text<
                                                text_placement_down,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O'>
                                                >
                                        >
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_b<bond_type::plain>,
                                        basic_text<
                                                text_placement_right,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O'>
                                                >,
                                                basic_text_span<
                                                        text_style_high_nm,
                                                        basic_text_rep<'-'>
                                                >
                                        >
                                >
                        >
                >
        >;

using cysteine =
        basic_amino_acid<
                'C',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_text<
                                text_placement_down_right,
                                basic_text_span<
                                        text_style_normal,
                                        basic_text_rep<'S', 'H'>
                                >
                        >
                >
        >;

using glutamine =
        basic_amino_acid<
                'Q',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_a<bond_type::double_bond>,
                                        basic_text<
                                                text_placement_down_left,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O'>
                                                >
                                        >
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_b<bond_type::plain>,
                                        basic_text<
                                                text_placement_down_right,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'N', 'H'>
                                                >,
                                                basic_text_span<
                                                        text_style_small,
                                                        basic_text_rep<'2'>
                                                >
                                        >
                                >
                        >
                >
        >;

using glutamate =
        basic_amino_acid<
                'E',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_a<bond_type::double_bond>,
                                        basic_text<
                                                text_placement_down_left,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O'>
                                                >
                                        >
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_b<bond_type::plain>,
                                        basic_text<
                                                text_placement_down_right,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O'>
                                                >,
                                                basic_text_span<
                                                        text_style_high_nm,
                                                        basic_text_rep<'-'>
                                                >
                                        >
                                >
                        >
                >
        >;

using glycine =
        basic_amino_acid<
                'G',
                bond_type::plain,
                basic_side_chain<>
        >;

using isoleucine =
        basic_amino_acid<
                'I',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_a<bond_type::wedged> // wedged on purpose
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_b<bond_type::plain>,
                                        flip_flip,
                                        basic_side_chain_bond<bond_type::plain>
                                >
                        >
                >
        >;

using leucine =
        basic_amino_acid<
                'L',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_a<bond_type::plain>
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_hard_b<bond_type::plain>
                                >
                        >
                >
        >;

using lysine =
        basic_amino_acid<
                'K',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_text<
                                text_placement_down,
                                basic_text_span<
                                        text_style_high_nm,
                                        basic_text_rep<'+'>
                                >,
                                basic_text_span<
                                        text_style_normal,
                                        basic_text_rep<'N', 'H'>,
                                        flip_style<text_style_high_nm>
                                >,
                                basic_text_span<
                                        text_style_small,
                                        basic_text_rep<'3'>
                                >
                        >
                >
        >;

using methionine =
        basic_amino_acid<
                'M',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_text<
                                text_placement_down_left,
                                basic_text_span<
                                        text_style_normal,
                                        basic_text_rep<'S'>
                                >
                        >,
                        basic_side_chain_bond<bond_type::plain>
                >
        >;

using serine =
        basic_amino_acid<
                'S',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_bond<bond_type::plain>,
                        basic_text<
                                text_placement_down_right,
                                basic_text_span<
                                        text_style_normal,
                                        basic_text_rep<'O', 'H'>
                                >
                        >
                >
        >;

using threonine =
        basic_amino_acid<
                'T',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_a<bond_type::plain>
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_b<bond_type::plain>,
                                        basic_text<
                                                text_placement_down_right,
                                                basic_text_span<
                                                        text_style_normal,
                                                        basic_text_rep<'O', 'H'>
                                                >
                                        >
                                >
                        >
                >
        >;

using valine =
        basic_amino_acid<
                'V',
                bond_type::dashed,
                basic_side_chain<
                        basic_side_chain_bond<bond_type::plain>,
                        basic_side_chain_split<
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_a<bond_type::plain>
                                >,
                                basic_side_chain<
                                        basic_side_chain_split_bond_soft_b<bond_type::plain>
                                >
                        >
                >
        >;

} // namespace bio::skeletal::defs