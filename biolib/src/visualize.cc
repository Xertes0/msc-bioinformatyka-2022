#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#include <cmath>
#include <numeric>
#include <ostream>
#include <string_view>

double
to_radians(double a)
{
    return (a/180.0)*std::numbers::pi_v<double>;
}

enum class
bond_type
{
    plain,
    wedged,
    dashed,
    // wavy,
    double_bond,
};

// First bond (plain or wedged:dashed)
// Side chain
// Always has double bond Oxygen
static constexpr int BOND_LENGTH = 25;

struct
draw_ctx
{
    bool flip{false};
    double x{50};
    double y{50};
};

void
draw_bond(draw_ctx& ctx, bond_type bond, double rot_a, double rot_b = std::numeric_limits<double>::max())
{
    //ctx.rot += rot_a * (ctx.flip?-1:1) + (ctx.flip?90-rot_a:0);
    double rot{};
    if(rot_b != std::numeric_limits<double>::max()) {
        rot = (ctx.flip?rot_b:rot_a);
    } else {
        rot = (-45 + rot_a) * (ctx.flip ? -1 : 1);
    }

    switch(bond) {
        case bond_type::plain: {
            std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n", ctx.x, ctx.y,
                ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH),
                ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH)
            );
            break;
        }

        case bond_type::dashed: {
            auto old_xa = ctx.x + (std::cos(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
            auto old_ya = ctx.y + (std::sin(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

            auto old_xb = ctx.x + (std::cos(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
            auto old_yb = ctx.y + (std::sin(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));

            std::printf("<polygon points='%f %f, %f %f, %f %f' fill='url(#bond-dashed)' />\n",
                old_xa, old_ya,
                old_xb, old_yb,
                ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH),
                ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH)
            );
            break;
        }

        case bond_type::double_bond:
        {
            auto draw = [&](double move_dir) {
                auto offset_x = (std::cos(to_radians(rot + (90.0 * move_dir))));
                auto offset_y = (std::sin(to_radians(rot + (90.0 * move_dir))));
                std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n",
                    ctx.x + offset_x,
                    ctx.y + offset_y,
                    ctx.x + (std::cos(to_radians(rot))*BOND_LENGTH) + offset_x,
                    ctx.y + (std::sin(to_radians(rot))*BOND_LENGTH) + offset_y
                );};

            draw(-1.0);
            draw( 1.0);

            break;
        }

        default: throw std::runtime_error{"bond not implemented"};
    }

    ctx.x += (std::cos(to_radians(rot))*BOND_LENGTH);
    ctx.y += (std::sin(to_radians(rot))*BOND_LENGTH);
    ctx.flip = !ctx.flip;
}

void
draw_text()
{

}

template<bond_type BondType>
struct
sc_bond_descriptor
{
    static
    void
    draw(draw_ctx& ctx, bool flip)
    {
        if(flip) {
            draw_bond(ctx, BondType, 90, -45 + 80);
        } else {
            draw_bond(ctx, BondType, 360 - 90, 360 - (-45 + 80));
        }
    }
};

template<class... Args>
struct
side_chain_descriptor
{
    static
    void
    draw(draw_ctx& ctx)
    {
        auto flip = ctx.flip;
        ctx.flip = false;
        (Args::draw(ctx, flip), ...);
    }
};

template<bond_type FirstBond, class SideChainDesc>
struct
amino_acid_descriptor
{
    static
    void
    draw(draw_ctx& ctx)
    {
        draw_bond(ctx, FirstBond, 80);
        //draw_bond(ctx, bond_type::plain, -45 + 90);
        auto side_chain = draw_ctx{ctx};
        draw_bond(ctx, bond_type::plain, 80);
        auto dbond = draw_ctx{ctx};
        draw_bond(ctx, bond_type::plain, 80);

        draw_bond(dbond, bond_type::double_bond, -45);
        SideChainDesc::draw(side_chain);
    }
};

using glycine = amino_acid_descriptor<bond_type::plain, side_chain_descriptor<>>;
using alanine = amino_acid_descriptor<bond_type::plain, side_chain_descriptor<sc_bond_descriptor<bond_type::wedged>>>;
using cysteine =
    amino_acid_descriptor<
        bond_type::dashed,
        side_chain_descriptor<
            sc_bond_descriptor<bond_type::plain>,
            sc_bond_descriptor<bond_type::plain>
        >
    >;

namespace svg
{

struct
draw_context
{
    double rot_{0};
    double x_{0};
    double y_{0};

    double length_{25};

    auto&
    bond(double rot, bond_type bond = bond_type::plain)
    {
        rot_ += rot;

        switch(bond) {
            case bond_type::plain:
            {
                std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n", x_, y_,
                    x_ + (std::cos(to_radians(rot_))*length_),
                    y_ + (std::sin(to_radians(rot_))*length_)
                );
                break;
            }

            case bond_type::double_bond:
            {
                auto draw = [&](double move_dir) {
                    auto offset_x = (std::cos(to_radians(rot_ + (90.0 * move_dir))));
                    auto offset_y = (std::sin(to_radians(rot_ + (90.0 * move_dir))));
                    std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n",
                        x_ + offset_x,
                        y_ + offset_y,
                        x_ + (std::cos(to_radians(rot_))*length_) + offset_x,
                        y_ + (std::sin(to_radians(rot_))*length_) + offset_y
                );};

                draw(-1.0);
                draw( 1.0);

                break;
            }

            case bond_type::dashed:
            {

                auto old_xa = x_ + (std::cos(to_radians(rot_-90))*(length_*0.1));
                auto old_ya = y_ + (std::sin(to_radians(rot_-90))*(length_*0.1));

                auto old_xb = x_ + (std::cos(to_radians(rot_+90))*(length_*0.1));
                auto old_yb = y_ + (std::sin(to_radians(rot_+90))*(length_*0.1));

                std::printf("<polygon points='%f %f, %f %f, %f %f' fill='url(#bond-dashed)' />\n",
                    old_xa, old_ya,
                    old_xb, old_yb,
                    x_ + (std::cos(to_radians(rot_))*length_),
                    y_ + (std::sin(to_radians(rot_))*length_)
                );
                break;
            }

            default: throw std::runtime_error{"not implemented"};
        }

        x_ += (std::cos(to_radians(rot_))*length_);
        y_ += (std::sin(to_radians(rot_))*length_);

        return *this;
    }

    auto&
    text(char const* str, double offset_x, double offset_y)
    {
        std::printf("<text x='%f' y='%f'>%s</text>\n", x_ + offset_x, y_ + offset_y, str);
        return *this;
    }

    auto&
    split(auto lamb)
    {
        lamb(draw_context{*this});
        return *this;
    }
};

}

void
draw_c()
{
    svg::draw_context ctx{};
    ctx.rot_ = 45;
    ctx.x_ = 50;
    ctx.y_ = 50;

    ctx
        .text("H", -3.5, -12.0)
        .text("N", -3.5, 0)
        .bond(0, bond_type::dashed)
        .split([](auto ctx) {
            ctx
                .bond(45)
                .bond(-45)
                .text("SH", 0, 11);
        })
        .bond(-90)
        .split([](auto ctx) {
            ctx
                .bond(-45, bond_type::double_bond)
                .text("O", -3.5, -1);
        })
        .bond(90)
        .text("N", -3.5, 11.0)
        .text("H", -3.5, 23.0);
}

int main()
{
    std::printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    std::printf("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>\n");
    std::printf("<defs><pattern id='bond-dashed' height='10%%' width='10%%'><line x1='0' x2='0' y1='0' y2='10' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    //draw_c();
    draw_ctx ctx{};
    cysteine::draw(ctx);
    cysteine::draw(ctx);

    std::printf("</svg>\n");

    return 0;
}

#pragma clang diagnostic pop