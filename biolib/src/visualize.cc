#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#include <cmath>
#include <numeric>
#include <ostream>
#include <string_view>

constexpr
double
to_radians(double degrees)
{
    return (degrees/180.0)*std::numbers::pi_v<double>;
}

static constexpr int SINGLE_CHAR_OFFSET_X = 4;
static constexpr int SINGLE_CHAR_OFFSET_Y = 8;

enum class
bond_type
{
    plain,
    wedged,
    dashed,
    // wavy,
    double_bond,
};

static constexpr int BOND_LENGTH = 25;

struct
draw_context
{
    bool flip{false};
    double x{75};
    double y{75};
};

void
draw_bond(draw_context& ctx, bond_type bond, double rot_a, double rot_b = std::numeric_limits<double>::max(), bool reverse = false)
{
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
        double x1,x2,x3,y1,y2,y3; // NOLINT(readability-isolate-declaration,cppcoreguidelines-init-variables,readability-identifier-length)

        if(reverse) {
            x1 = ctx.x;
            y1 = ctx.y;

            x2 = ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH) + (std::cos(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
            y2 = ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH) + (std::sin(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

            x3 = ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH) + (std::cos(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
            y3 = ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH) + (std::sin(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
        } else {
            x1 = ctx.x + (std::cos(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));
            y1 = ctx.y + (std::sin(to_radians(rot - 90)) * (BOND_LENGTH * 0.1));

            x2 = ctx.x + (std::cos(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));
            y2 = ctx.y + (std::sin(to_radians(rot + 90)) * (BOND_LENGTH * 0.1));

            x3 = ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH);
            y3 = ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH);
        }

        std::printf("<polygon points='%f %f, %f %f, %f %f' fill='%s' />\n",
            x1, y1,
            x2, y2,
            x3, y3,
            bond == bond_type::dashed ? "url(#bond-dashed)" : "black"
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

template<char A, char B>
struct
text_vert
{
    static
    void
    draw(draw_context& ctx, bool flip = false)
    {
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y -= (flip?0:SINGLE_CHAR_OFFSET_Y*2);
        std::printf("<text text-anchor='middle' x='%f' y='%f'><tspan dy='0.5em'>%c</tspan><tspan x='%f' dy='1em'>%c</tspan></text>",
            ctx.x,
            ctx.y,
            flip?B:A,
            ctx.x,
            flip?A:B
        );
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y += (flip?0:SINGLE_CHAR_OFFSET_Y*2);
    }
};

template<char A, int OffsetX, int OffsetY>
struct
text_single
{
    static
    void
    draw(draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        std::printf("<text text-anchor='middle' dominant-baseline='middle' x='%f' y='%f'>%c</text>",
            ctx.x,
            ctx.y,
            A
        );
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<char A, char B, int OffsetX, int OffsetY>
struct
text_hor
{
    static
    void
    draw(draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        std::printf("<text text-anchor='middle' dominant-baseline='middle' x='%f' y='%f'>%c%c</text>",
            ctx.x,
            ctx.y,
            A,
            B
        );
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<bond_type BondType>
struct
basic_side_chain_bond
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
            draw_bond(ctx, bond, 90, -45 + 80, true);
        } else {
            draw_bond(ctx, bond, 360 - 90, 360 - (-45 + 80), true);
        }
    }
};

template<class... Args>
struct
basic_side_chain
{
    static
    void
    draw(draw_context& ctx)
    {
        [[maybe_unused]] auto flip = ctx.flip;
        ctx.flip = false;
        (Args::draw(ctx, flip), ...);
    }
};

template<bond_type FirstBond, class SideChainDesc>
struct
basic_amino_acid
{
    static
    void
    draw(draw_context& ctx)
    {
        text_vert<'H', 'N'>::draw(ctx, ctx.flip);

        auto first_bond = FirstBond;
        if(ctx.flip) {
            if(first_bond == bond_type::wedged) {
                first_bond = bond_type::dashed;
            } else if(first_bond == bond_type::dashed) {
                first_bond = bond_type::wedged;
            }
        }

        draw_bond(ctx, first_bond, 80);
        auto side_chain = draw_context{ctx};
        draw_bond(ctx, bond_type::plain, 80);
        auto dbond = draw_context{ctx};
        draw_bond(ctx, bond_type::plain, 80);

        draw_bond(dbond, bond_type::double_bond, -45);
        text_single<'O', 0, SINGLE_CHAR_OFFSET_Y>::draw(dbond, ctx.flip);
        SideChainDesc::draw(side_chain);
    }
};

using alanine =
    basic_amino_acid<
        bond_type::plain,
        basic_side_chain<
            basic_side_chain_bond<bond_type::wedged>
        >
    >;

using cysteine =
    basic_amino_acid<
        bond_type::dashed,
        basic_side_chain<
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            text_hor<'S', 'H', SINGLE_CHAR_OFFSET_X*2, 0>
        >
    >;

using glycine =
    basic_amino_acid<
        bond_type::plain,
        basic_side_chain<>
    >;

int main()
{
    std::printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    std::printf("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>\n");
    std::printf("<defs><pattern id='bond-dashed' height='10%%' width='10%%'><line x1='0' y1='0' x2='10' y2='0' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    draw_context ctx{};

    text_single<'H', SINGLE_CHAR_OFFSET_X, -(SINGLE_CHAR_OFFSET_Y*2)>::draw(ctx);
    std::printf("<text text-anchor='left' dominant-baseline='middle' x='%f' y='%f' style='font-size:6px;'>2</text>",
        ctx.x,
        ctx.y + SINGLE_CHAR_OFFSET_Y*2.5
    );
    text_single<'+', SINGLE_CHAR_OFFSET_X, SINGLE_CHAR_OFFSET_Y>::draw(ctx);

    cysteine::draw(ctx);
    cysteine::draw(ctx);
    alanine::draw(ctx);
    alanine::draw(ctx);
    glycine::draw(ctx);
    glycine::draw(ctx);

    text_single<'O', SINGLE_CHAR_OFFSET_X, 0>::draw(ctx);
    text_single<'-', SINGLE_CHAR_OFFSET_X, -SINGLE_CHAR_OFFSET_Y>::draw(ctx);

    std::printf("</svg>\n");

    return 0;
}

#pragma clang diagnostic pop