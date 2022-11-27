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
draw_context
{
    bool flip{false};
    double x{50};
    double y{50};
};

void
draw_bond(draw_context& ctx, bond_type bond, double rot_a, double rot_b = std::numeric_limits<double>::max())
{
    //ctx.rot += rot_a * (ctx.flip?-1:1) + (ctx.flip?90-rot_a:0);
    double rot{};
    if(rot_b != std::numeric_limits<double>::max()) {
        rot = (ctx.flip?rot_b:rot_a);
    } else {
        rot = (-45 + rot_a) * (ctx.flip ? -1 : 1);
    }

    if(bond == bond_type::plain) {
        std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n", ctx.x, ctx.y,
            ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH),
            ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH)
        );
    } else if(bond == bond_type::dashed || bond == bond_type::wedged) {
        auto old_xa = ctx.x + (std::cos(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
        auto old_ya = ctx.y + (std::sin(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

        auto old_xb = ctx.x + (std::cos(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
        auto old_yb = ctx.y + (std::sin(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));

        std::printf("<polygon points='%f %f, %f %f, %f %f' fill='%s' />\n",
            old_xa, old_ya,
            old_xb, old_yb,
            ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH),
            ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH),
            bond==bond_type::dashed?"url(#bond-dashed)":"black"
        );
    } else if(bond == bond_type::double_bond) {
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
    } else {
        throw std::runtime_error{"bond not implemented"};
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
    draw(draw_context& ctx, bool flip)
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
            draw_bond(ctx, bond, 90, -45 + 80);
        } else {
            draw_bond(ctx, bond, 360 - 90, 360 - (-45 + 80));
        }
    }
};

template<class... Args>
struct
side_chain_descriptor
{
    static
    void
    draw(draw_context& ctx)
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
    draw(draw_context& ctx)
    {
        draw_bond(ctx, FirstBond, 80);
        //draw_bond(ctx, bond_type::plain, -45 + 90);
        auto side_chain = draw_context{ctx};
        draw_bond(ctx, bond_type::plain, 80);
        auto dbond = draw_context{ctx};
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

int main()
{
    std::printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    std::printf("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>\n");
    std::printf("<defs><pattern id='bond-dashed' height='10%%' width='10%%'><line x1='0' y1='0' x2='10' y2='0' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    //draw_c();
    draw_context ctx{};
    cysteine::draw(ctx);
    cysteine::draw(ctx);
    alanine::draw(ctx);
    alanine::draw(ctx);

    std::printf("</svg>\n");

    return 0;
}

#pragma clang diagnostic pop