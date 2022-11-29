#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"

#include <array>
#include <cassert>
#include <cmath>
#include <iterator>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string_view>
#include <tuple>

constexpr
double
to_radians(double degrees)
{
    return (degrees / 180.0) * std::numbers::pi_v<double>;
}

constexpr
void
format(std::string& str, double val)
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

constexpr
void
format(std::string& str, char val)
{
    str.push_back(val);
}

template<std::size_t N>
constexpr
void
format(std::string& str, char const (&val)[N]) // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
{
    str.append(val);
}

constexpr
void
format(std::string& str, char const* val)
{
    str.append(val);
}

template<class... Args>
constexpr
void
format(std::string& str, Args... args)
{
    (format(str, args), ...);
}

static constexpr std::size_t BUFFER_SIZE = 1024*15;
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

static constexpr double DRAW_CTX_OFFSET_X = 125;
static constexpr double DRAW_CTX_OFFSET_Y = 125;

struct
draw_context
{
    bool flip{false};
    double x{DRAW_CTX_OFFSET_X};
    double y{DRAW_CTX_OFFSET_Y};
};

constexpr
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
        format(str,
            "<line x1='", ctx.x, "' y1='", ctx.y, "' x2='", ctx.x + (std::cos(to_radians(rot)) * BOND_LENGTH), "' y2='", ctx.y + (std::sin(to_radians(rot)) * BOND_LENGTH), "' stroke='black' />\n");
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

        format(str, "<polygon points='", x1, " ", y1, ", ", x2, " ", y2, ", ", x3, " ", y3, "' fill='", bond == bond_type::dashed ? "url(#bond-dashed)" : "black", "' />\n");
    } else if(bond == bond_type::double_bond) {
        auto draw = [&] (double move_dir) constexpr {
            auto offset_x = (std::cos(to_radians(rot + (90.0 * move_dir))));
            auto offset_y = (std::sin(to_radians(rot + (90.0 * move_dir))));

            format(str, "<line x1='", ctx.x + offset_x, "' y1='", ctx.y + offset_y, "' x2='", ctx.x + (std::cos(to_radians(rot))*BOND_LENGTH) + offset_x, "' y2='", ctx.y + (std::sin(to_radians(rot))*BOND_LENGTH) + offset_y, "' stroke='black' />\n");
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

template<char A, char B, int AddY = 0>
struct
text_vert
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip = false)
    {
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y -= AddY * (flip?-1:1);
        ctx.y -= (flip?0:SINGLE_CHAR_OFFSET_Y*2);
        format(str, "<text text-anchor='middle' x='", ctx.x, "' y='", ctx.y, "'><tspan dy='0.5em'>", flip?B:A, "</tspan><tspan x='", ctx.x, "' dy='1em'>", flip?A:B, "</tspan></text>");
        ctx.x += SINGLE_CHAR_OFFSET_X;
        ctx.y += (flip?0:SINGLE_CHAR_OFFSET_Y*2);
        ctx.y += AddY * (flip?-1:1);
    }
};

template<char A, int OffsetX, int OffsetY>
struct
text_single
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        format(str, "<text text-anchor='middle' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y, "'>", A, "</text>");
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<char A, char B, int OffsetX, int OffsetY>
struct
text_single_with_smol
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        format(str, "<text text-anchor='left' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y, "'><tspan>", A, "</tspan><tspan dy='-0.5em'>", B, "</tspan></text>");
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
    }
};

template<char A, int OffsetX, int OffsetY>
struct
text_single_smol
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?6.2:0);
        format(str, "<text style='font-size:6px;' text-anchor='left' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y, "'>", A, "</text>");
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?6.2:0);
    }
};

template<char A, char B, int OffsetX, int OffsetY>
struct
text_hor
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool dir = false)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
        format(str, "<text text-anchor='middle' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y, "'>", A, B, "</text>");
        ctx.x += OffsetX;
        ctx.y += OffsetY * (dir?-1:1) + (dir?2:0);
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

template<class SvgId, bond_type FirstBond, class SideChainDesc>
struct
basic_amino_acid
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx)
    {
        format(str, "<symbol id='aa_cache-", static_cast<char>(48 + SvgId::value), ctx.flip?"f":"", "'>\n");
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

        format(str, "</symbol>\n");
    }
};

//template<class... Args>
//struct
//basic_side_chain_split_component
//{
//    [[maybe_unused]] auto flip = ctx.flip;
//    ctx.flip = false;
//    (Args::draw(str, ctx, flip), ...);
//};

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

struct alaine_svg_id { static constexpr int value{0}; };
using alanine =
    basic_amino_acid<
        alaine_svg_id,
        bond_type::plain,
        basic_side_chain<
            basic_side_chain_bond<bond_type::wedged>
        >
    >;

struct aspariqine_svg_id { static constexpr int value{4}; };
using aspariqine =
    basic_amino_acid<
        aspariqine_svg_id,
        bond_type::dashed,
        basic_side_chain<
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_split<
                basic_side_chain<
                    basic_side_chain_bond<bond_type::double_bond, 90>,
                    text_single<'O', 0, (-(SINGLE_CHAR_OFFSET_Y)*2)/3>
                >,
                basic_side_chain<
                    basic_side_chain_bond<bond_type::plain, 0>,
                    text_hor<'N', 'H', SINGLE_CHAR_OFFSET_X*2, 0>,
                    text_single_smol<'2', 0, SINGLE_CHAR_OFFSET_Y/2>
                >
            >
        >
    >;

struct aspartate_svg_id { static constexpr int value{5}; };
using aspartate =
    basic_amino_acid<
        aspartate_svg_id,
        bond_type::dashed,
        basic_side_chain<
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_split<
                basic_side_chain<
                    basic_side_chain_bond<bond_type::double_bond, 90>,
                    text_single<'O', 0, (-(SINGLE_CHAR_OFFSET_Y)*2)/3>
                >,
                basic_side_chain<
                    basic_side_chain_bond<bond_type::plain, 0>,
                    text_single_with_smol<'O', '-', 0, 0>
                >
            >
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

struct glutamine_svg_id { static constexpr int value{3}; };
using glutamine =
    basic_amino_acid<
        glutamine_svg_id,
        bond_type::dashed,
        basic_side_chain<
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_bond<bond_type::plain>,
            basic_side_chain_split<
                basic_side_chain<
                    basic_side_chain_bond<bond_type::double_bond, 135>,
                    text_single<'O', -SINGLE_CHAR_OFFSET_X, -(SINGLE_CHAR_OFFSET_Y)/2>
                >,
                basic_side_chain<
                    basic_side_chain_bond<bond_type::plain, 45>,
                    text_hor<'N', 'H', SINGLE_CHAR_OFFSET_X*2, -(SINGLE_CHAR_OFFSET_Y)/2>,
                    text_single_smol<'2', 0, SINGLE_CHAR_OFFSET_Y>
                >
            >
        >
    >;

consteval
std::tuple<buffer_t, std::size_t>
cache_header()
{
    std::string str{};

    str.append("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    str.append("<style>text {font-family:monospace;font-size:12px;font-weight:bold;}</style>\n");
    str.append("<defs><pattern id='bond-dashed' height='10%' width='10%'><line x1='0' y1='0' x2='10' y2='0' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    auto append = [&]<class AminoAcid>([[maybe_unused]] AminoAcid) consteval {
        draw_context ctx{};
        AminoAcid::draw(str, ctx);
        str.push_back('\n');

        ctx = draw_context{};
        ctx.flip = true;
        AminoAcid::draw(str, ctx);
        str.push_back('\n');
    };

    append(alanine{});
    append(aspariqine{});
    append(aspartate{});
    append(cysteine{});
    append(glycine{});
    append(glutamine{});

    draw_context ctx{};
    ctx.x -= SINGLE_CHAR_OFFSET_X*4;
    ctx.y += SINGLE_CHAR_OFFSET_Y*2;
    text_single<'H', SINGLE_CHAR_OFFSET_X, -(SINGLE_CHAR_OFFSET_Y*2)>::draw(str, ctx);
    format(str, "<text text-anchor='left' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y + SINGLE_CHAR_OFFSET_Y*2.5, "' style='font-size:6px;'>2</text>");
    text_single<'+', SINGLE_CHAR_OFFSET_X, SINGLE_CHAR_OFFSET_Y>::draw(str, ctx);

    buffer_t buf{};
    //assert(buf.size() <= str.length());
    std::copy(str.begin(), str.end(), buf.data());

    return {buf, str.length()};
}

void
draw(std::stringstream& sstream, draw_context& ctx, std::size_t index)
{
    sstream <<
        "<use href='#aa_cache-" <<
        index <<
        (ctx.flip?"f":"") <<
        "' x='" <<
        ctx.x - DRAW_CTX_OFFSET_X <<
        "' y='" <<
        ctx.y - DRAW_CTX_OFFSET_Y <<
        "' />\n";

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
        case 'Q': return 3;
        case 'N': return 4;
        case 'D': return 5;
        default: throw std::runtime_error{"Bad ctoaacai value"};
    }
}

int main()
{
    auto [header_arr, header_length] = cache_header();
    std::stringstream sstream{};
    std::copy(header_arr.begin(), header_arr.begin()+header_length, std::ostream_iterator<char>(sstream));

    draw_context ctx{};

    //std::string buf{};

    draw(sstream, ctx, ctoaacai('D'));
    draw(sstream, ctx, ctoaacai('D'));
    draw(sstream, ctx, ctoaacai('N'));
    draw(sstream, ctx, ctoaacai('N'));
    draw(sstream, ctx, ctoaacai('Q'));
    draw(sstream, ctx, ctoaacai('Q'));
    draw(sstream, ctx, ctoaacai('C'));
    draw(sstream, ctx, ctoaacai('C'));
    draw(sstream, ctx, ctoaacai('A'));
    draw(sstream, ctx, ctoaacai('G'));
    draw(sstream, ctx, ctoaacai('G'));
    draw(sstream, ctx, ctoaacai('A'));

    auto str = sstream.str();
    text_single<'O', SINGLE_CHAR_OFFSET_X, 0>::draw(str, ctx);
    text_single<'-', SINGLE_CHAR_OFFSET_X, -SINGLE_CHAR_OFFSET_Y>::draw(str, ctx);

    //sstream << "</svg>\n";
    str.append("</svg>\n");

    std::printf("%s\n", str.c_str());

    return 0;
}

#pragma clang diagnostic pop