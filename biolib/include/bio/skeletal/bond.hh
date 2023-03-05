#pragma once

#include <string>
#include <limits>
#include <stdexcept>

#include <gcem.hpp>

#include "bio/skeletal/detail/format.hh"
#include "bio/skeletal/detail/radian.hh"
#include "bio/skeletal/draw_context.hh"

namespace bio::skeletal
{

static constexpr int BOND_LENGTH = 25;

enum class
bond_type
{
    plain,
    wedged,
    dashed,
    // wavy,
    double_bond,
    half_double,
};

template<int BOND_LENGTH = ::bio::skeletal::BOND_LENGTH>
constexpr
void
draw_bond(std::string &str, draw_context &ctx, bond_type bond, double rot_a,
          double rot_b = std::numeric_limits<double>::max(), bool reverse = false)
{
    double rot{};
    if(rot_b != std::numeric_limits<double>::max()) {
        rot = (ctx.flip ? rot_b : rot_a);
    } else {
        rot = (-45 + rot_a) * (ctx.flip ? -1 : 1);
    }

    if(bond == bond_type::plain) {
        detail::format(str,
                       "<line x1='", ctx.x, "' y1='", ctx.y, "' x2='",
                       ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH), "' y2='",
                       ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH), "' stroke='black'></line>");
    } else if(bond == bond_type::dashed || bond == bond_type::wedged) {
        double x1, x2, x3, y1, y2, y3; // NOLINT(readability-isolate-declaration,cppcoreguidelines-init-variables,readability-identifier-length)

        if(reverse) {
            x1 = ctx.x;
            y1 = ctx.y;

            x2 = ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH) +
                 (gcem::cos(detail::to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
            y2 = ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH) +
                 (gcem::sin(detail::to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

            x3 = ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH) +
                 (gcem::cos(detail::to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
            y3 = ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH) +
                 (gcem::sin(detail::to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
        } else {
            x1 = ctx.x + (gcem::cos(detail::to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
            y1 = ctx.y + (gcem::sin(detail::to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

            x2 = ctx.x + (gcem::cos(detail::to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
            y2 = ctx.y + (gcem::sin(detail::to_radians(rot + 90)) * (BOND_LENGTH * 0.1));

            x3 = ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH);
            y3 = ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH);
        }

        detail::format(str, "<polygon points='", x1, " ", y1, ", ", x2, " ", y2, ", ", x3, " ", y3, "' fill='",
                       bond == bond_type::dashed ? "url(#bond-dashed)" : "black", "'></polygon>");
    } else if(bond == bond_type::double_bond) {
        auto draw = [&](double move_dir) constexpr {
            auto offset_x = (gcem::cos(detail::to_radians(rot + (90.0 * move_dir))));
            auto offset_y = (gcem::sin(detail::to_radians(rot + (90.0 * move_dir))));

            detail::format(str, "<line x1='", ctx.x + offset_x, "' y1='", ctx.y + offset_y, "' x2='",
                           ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH) + offset_x, "' y2='",
                           ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH) + offset_y, "' stroke='black'></line>");
        };

        draw(-1.0);
        draw(1.0);
    } else if(bond == bond_type::half_double) {
        auto draw = [&](double move_dir) constexpr {
            auto offset_x = (gcem::cos(detail::to_radians(rot + (90.0 * move_dir))));
            auto offset_y = (gcem::sin(detail::to_radians(rot + (90.0 * move_dir))));

            detail::format(str, "<line x1='", ctx.x + offset_x, "' y1='", ctx.y + offset_y, "' x2='",
                           ctx.x + (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH) + offset_x, "' y2='",
                           ctx.y + (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH) + offset_y, "' stroke='black' ", move_dir==-1.0?"stroke-dasharray=\"2\"":"", "></line>");
        };

        draw(-1.0);
        draw(1.0);
    } else {
        throw std::runtime_error{"bond not implemented"};
    }

    ctx.x += (gcem::cos(detail::to_radians(rot)) * BOND_LENGTH);
    ctx.y += (gcem::sin(detail::to_radians(rot)) * BOND_LENGTH);
    ctx.flip = !ctx.flip;
}

} // namespace bio::skeletal
