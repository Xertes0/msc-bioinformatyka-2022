#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <array>
#include <cassert>
#include <cmath>
#include <numeric>
#include <ostream>
#include <string_view>

#include <fmt/format.h>

constexpr
double
to_radians(double degrees)
{
    return (degrees / 180.0) * std::numbers::pi_v<double>;
}

constexpr
void
format_f(std::string& str, double val)
{
    int nat = static_cast<int>(val);
    int fra = static_cast<int>((val - nat) * 1'000'000);
    std::string buf{"."};

    while(nat != 0) {
        buf.push_back(static_cast<char>(48 + (nat % 10)));
        nat /= 10;
    }
    if(buf.ends_with('.')) {
        buf.push_back('0');
    }
    str.append(std::string{buf.rbegin(), buf.rend()});

    buf.clear();

    while(fra != 0) {
        buf.push_back(static_cast<char>(48 + (fra % 10)));
        fra /= 10;
    }
    if(buf.empty()) {
        str.push_back('0');
    } else {
        str.append(std::string{buf.rbegin(), buf.rend()});
    }
}

static constexpr std::size_t BUFFER_SIZE = 1024*5;
using buffer_t = std::array<char, BUFFER_SIZE>;

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

static constexpr double DRAW_CTX_OFFSET_X = 75;
static constexpr double DRAW_CTX_OFFSET_Y = 75;

struct
draw_context
{
    bool flip{false};
    double x{DRAW_CTX_OFFSET_X};
    double y{DRAW_CTX_OFFSET_Y};
};

consteval
void
draw_bond(std::string& str, draw_context& ctx, bond_type bond, double rot_a, double rot_b = std::numeric_limits<double>::max(), bool reverse = false)
{
    double rot{};
    if(rot_b != std::numeric_limits<double>::max()) {
        rot = (ctx.flip?rot_b:rot_a);
    } else {
        rot = (-45 + rot_a) * (ctx.flip ? -1 : 1);
    }

    if(bond == bond_type::plain) {
        str.append("<line x1='");
        format_f(str, ctx.x);
        str.append("' y1='");
        format_f(str, ctx.y);
        str.append("' x2='");
        format_f(str, ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH));
        str.append("' y2='");
        format_f(str, ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH));
        str.append("' stroke='black' />\n");
        //str.append(fmt::format("<line x1='{}' y1='{}' x2='{}' y2='{}' stroke='black' />\n", ctx.x, ctx.y,
        //    ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH),
        //    ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH)
        //));
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

        str.append("<polygon points='");
        format_f(str, x1);
        str.append(" ");
        format_f(str, y1);
        str.append(", ");
        format_f(str, x2);
        str.append(" ");
        format_f(str, y2);
        str.append(", ");
        format_f(str, x3);
        str.append(" ");
        format_f(str, y3);
        str.append("' fill='");
        str.append(bond == bond_type::dashed ? "url(#bond-dashed)" : "black");
        str.append("' />\n");
        //str.append(fmt::format("<polygon points='{} {}, {} {}, {} {}' fill='{}' />\n",
        //    x1, y1,
        //    x2, y2,
        //    x3, y3,
        //    bond == bond_type::dashed ? "url(#bond-dashed)" : "black"
        //));
    } else if(bond == bond_type::double_bond) {
        auto draw = [&] (double move_dir) consteval {
            auto offset_x = (std::cos(to_radians(rot + (90.0 * move_dir))));
            auto offset_y = (std::sin(to_radians(rot + (90.0 * move_dir))));
            str.append("<line x1='");
            format_f(str, ctx.x + offset_x);
            str.append("' y1='");
            format_f(str, ctx.y + offset_y);
            str.append("' x2='");
            format_f(str, ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH) + offset_x);
            str.append("' y2='");
            format_f(str, ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH) + offset_y);
            str.append("' stroke='black' />\n");
            //str.append(fmt::format("<line x1='{}' y1='{}' x2='{}' y2='{}' stroke='black' />\n",
            //    ctx.x + offset_x,
            //    ctx.y + offset_y,
            //    ctx.x + (std::cos(to_radians(rot))*BOND_LENGTH) + offset_x,
            //    ctx.y + (std::sin(to_radians(rot))*BOND_LENGTH) + offset_y
            //));
        };

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
    consteval
    void
    draw(std::string& str, draw_context& ctx, bool flip = false)
    {
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y -= (flip?0:SINGLE_CHAR_OFFSET_Y*2);
        str.append("<text text-anchor='middle' x='");
        format_f(str, ctx.x);
        str.append("' y='");
        format_f(str, ctx.y);
        str.append("'><tspan dy='0.5em'>");
        str.push_back(flip?B:A);
        str.append("</tspan><tspan x='");
        format_f(str, ctx.x);
        str.append("' dy='1em'>");
        str.push_back(flip?A:B);
        str.append("</tspan></text>");
        //str.append(fmt::format("<text text-anchor='middle' x='{}' y='{}'><tspan dy='0.5em'>{}</tspan><tspan x='{}' dy='1em'>{}</tspan></text>",
        //    ctx.x,
        //    ctx.y,
        //    flip?B:A,
        //    ctx.x,
        //    flip?A:B
        //));
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y += (flip?0:SINGLE_CHAR_OFFSET_Y*2);
    }
};

template<char A, int OffsetX, int OffsetY>
struct
text_single
{
    static
    consteval
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        str.append("<text text-anchor='middle' dominant-baseline='middle' x='");
        format_f(str, ctx.x);
        str.append("' y='");
        format_f(str, ctx.y);
        str.append("'>");
        str.push_back(A);
        str.append("</text>");
        //str.append(fmt::format("<text text-anchor='middle' dominant-baseline='middle' x='{}' y='{}'>{}</text>",
        //    ctx.x,
        //    ctx.y,
        //    A
        //));
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<char A, char B, int OffsetX, int OffsetY>
struct
text_hor
{
    static
    consteval
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        str.append("<text text-anchor='middle' dominant-baseline='middle' x='");
        format_f(str, ctx.x);
        str.append("' y='");
        format_f(str, ctx.y);
        str.append("'>");
        str.push_back(A);
        str.push_back(B);
        str.append("</text>");
        //str.append(fmt::format("<text text-anchor='middle' dominant-baseline='middle' x='{}' y='{}'>{}{}</text>",
        //    ctx.x,
        //    ctx.y,
        //    A,
        //    B
        //));
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<bond_type BondType>
struct
basic_side_chain_bond
{
    static
    consteval
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
            draw_bond(str, ctx, bond, 90, -45 + 80, true);
        } else {
            draw_bond(str, ctx, bond, 360 - 90, 360 - (-45 + 80), true);
        }
    }
};

template<class... Args>
struct
basic_side_chain
{
    static
    consteval
    void
    draw(std::string& str, draw_context& ctx)
    {
        [[maybe_unused]] auto flip = ctx.flip;
        ctx.flip = false;
        (Args::draw(str, ctx, flip), ...);
    }
};

template<class SvgId, bond_type FirstBond, class SideChainDesc>
struct
basic_amino_acid
{
    static
    consteval
    void
    draw(std::string& str, draw_context& ctx)
    {
        str.append("<symbol id='aa_cache-");
        str.push_back(static_cast<char>(48 + SvgId::value));
        str.append(ctx.flip?"f":"");
        str.append("'>\n");
        text_vert<'H', 'N'>::draw(str, ctx, ctx.flip);

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
        text_single<'O', 0, SINGLE_CHAR_OFFSET_Y>::draw(str, dbond, ctx.flip);
        SideChainDesc::draw(str, side_chain);

        str.append("</symbol>\n");
    }
};

struct alaine_svg_id { static constexpr int value{0}; };
using alanine =
    basic_amino_acid<
        alaine_svg_id,
        bond_type::plain,
        basic_side_chain<
            basic_side_chain_bond<bond_type::wedged>
        >
    >;

struct cysteine_svg_id { static constexpr int value{1}; };
using cysteine =
    basic_amino_acid<
        cysteine_svg_id,
        bond_type::dashed,
        basic_side_chain<
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            text_hor<'S', 'H', SINGLE_CHAR_OFFSET_X*2, 0>
        >
    >;

struct glycine_svg_id { static constexpr int value{2}; };
using glycine =
    basic_amino_acid<
        glycine_svg_id,
        bond_type::plain,
        basic_side_chain<>
    >;

template<class AminoAcid, bool Flip>
consteval
buffer_t
cache_amino_acid()
{
    std::string str{};
    draw_context ctx{};
    ctx.flip = Flip;
    AminoAcid::draw(str, ctx);

    assert(str.length() <= BUFFER_SIZE);
    buffer_t buffer{};
    std::copy(str.begin(), str.end(), buffer.data());

    return buffer;
}

#define UTIL_CACHE(NAME) \
    cache_amino_acid<NAME, false>(), \
    cache_amino_acid<NAME, true>(),

std::array<buffer_t, 6> AMINO_ACID_CACHE
{
    UTIL_CACHE(alanine)
    UTIL_CACHE(cysteine)
    UTIL_CACHE(glycine)
};

#undef UTIL_CACHE

void
draw(draw_context& ctx, std::size_t index)
{
    std::printf("<use href='#aa_cache-%li%s' x='%f' y='%f' />",
        index,
        ctx.flip?"f":"",
        ctx.x - DRAW_CTX_OFFSET_X,
        ctx.y - DRAW_CTX_OFFSET_Y
    );

    ctx.x += (std::cos(to_radians(-45+80)) * BOND_LENGTH * 3) + (SINGLE_CHAR_OFFSET_X * 2); // single char offset appears to be a half char offset
    ctx.y += (std::sin(to_radians(-45+80)) * BOND_LENGTH) * (ctx.flip?-1:1);
    ctx.flip = !ctx.flip;
}

constexpr
std::size_t
ctoaacai(char value)
{
    switch(value) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        default: throw std::runtime_error{"Bad ctoaacai value"};
    }
}

int main()
{
    std::printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    std::printf("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>\n");
    std::printf("<defs><pattern id='bond-dashed' height='10%%' width='10%%'><line x1='0' y1='0' x2='10' y2='0' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    draw_context ctx{};

    for(auto const& val : AMINO_ACID_CACHE) {
        std::printf("%s", val.data());
    }

    draw(ctx, ctoaacai('C'));
    draw(ctx, ctoaacai('C'));
    draw(ctx, ctoaacai('A'));
    draw(ctx, ctoaacai('G'));
    draw(ctx, ctoaacai('G'));
    draw(ctx, ctoaacai('A'));

    //draw_context ctx{};

    //text_single<'H', SINGLE_CHAR_OFFSET_X, -(SINGLE_CHAR_OFFSET_Y*2)>::draw(ctx);
    //std::printf("<text text-anchor='left' dominant-baseline='middle' x='%f' y='%f' style='font-size:6px;'>2</text>",
    //    ctx.x,
    //    ctx.y + SINGLE_CHAR_OFFSET_Y*2.5
    //);
    //text_single<'+', SINGLE_CHAR_OFFSET_X, SINGLE_CHAR_OFFSET_Y>::draw(ctx);

    //cysteine::draw(ctx);
    //cysteine::draw(ctx);
    //alanine::draw(ctx);
    //alanine::draw(ctx);
    //glycine::draw(ctx);
    //glycine::draw(ctx);

    //text_single<'O', SINGLE_CHAR_OFFSET_X, 0>::draw(ctx);
    //text_single<'-', SINGLE_CHAR_OFFSET_X, -SINGLE_CHAR_OFFSET_Y>::draw(ctx);

    std::printf("</svg>\n");

    return 0;
}

#pragma clang diagnostic pop