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

constexpr
void
format(std::string& str, std::string_view val)
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

static constexpr double TEXT_MARGIN = 2;

template<char... Args>
struct
text_rep
{
    static
    constexpr
    void
    append(std::string& str)
    {
        (str.push_back(Args), ...);
    }
};

struct
text_style_normal
{
    static
    constexpr
    std::string_view value{};
};

struct
text_style_small_straight
{
    static
    constexpr
    std::string_view value{"style='font-size:6px;' dy='1em'"};
};

struct
text_style_small_angle
{
    static
    constexpr
    std::string_view value{"style='font-size:6px;' dy='0.575em' dominant-baseline='mathematical'"};
};

struct
text_style_below
{
    static
    constexpr
    std::string_view value{"dx='-0.6em' dy='1em'"};
};

struct
text_style_high_nm
{
    static
    constexpr
    std::string_view value{"dy='-0.6em'"};
};

struct
text_style_above
{
    static
    constexpr
    std::string_view value{"dx='-0.6em' dy='-1em'"};
};

template<class TextStyle, class TextRep>
struct
basic_text_span
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        format(str, "<tspan ", TextStyle::value, ">");
        TextRep::append(str);
        format(str, "</tspan>");
    }
};

template<bool Right>
struct
basic_text_placement_straight
{
    static
    constexpr
    void
    append(std::string& str, draw_context& ctx, bool flip)
    {
        format(str,
               "text-anchor='",
               (Right?"start":"middle"),
               "' dominant-baseline='",
               (Right?"mathematical":(flip?"hanging":"text-top")),
               "' x='", ctx.x + (Right?TEXT_MARGIN: 0),
               "' y='", ctx.y + ((Right?0:TEXT_MARGIN) * (flip?1:-1)),
               "'");
    }
};

template<bool Right>
struct
basic_text_placement_angle
{
    static
    constexpr
    void
    append(std::string& str, draw_context& ctx, bool flip)
    {
        format(str,
               "text-anchor='",
               (Right?"start":"end"),
               "' dominant-baseline='",
               (flip?"hanging":"text-top"),
               "' x='", ctx.x + (Right?TEXT_MARGIN: 0),
               "' y='", ctx.y + ((Right?0:TEXT_MARGIN) * (flip?1:-1)),
               "'");
    }
};

using text_placement_right = basic_text_placement_straight<true>;
using text_placement_down  = basic_text_placement_straight<false>;
using text_placement_down_right = basic_text_placement_angle<true>;
using text_placement_down_left  = basic_text_placement_angle<false>;

template<class TextPlacement, class... Content>
struct
basic_text
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        format(str, "<text ");
        TextPlacement::append(str, ctx, flip);
        format(str, ">");
        (Content::draw(str, ctx, flip), ...);
        format(str, "</text>");
    }
};

template<int OffsetX, int OffsetY>//, class Suffix = text_rep<'p', 'x'>>
struct
fixed_offset
{
    static
    constexpr
    void
    draw(std::string& str, draw_context& ctx, bool flip)
    {
        ctx.x += OffsetX;
        ctx.y += OffsetY * (flip?1:-1);
    }
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

        if(ctx.flip) {
            basic_text<
                text_placement_right,
                basic_text_span<
                    text_style_normal,
                    text_rep<'N'>
                >,
                basic_text_span<
                    text_style_below,
                    text_rep<'H'>
                >
            >::draw(str, ctx, ctx.flip);
        } else {
            basic_text<
                text_placement_right,
                basic_text_span<
                    text_style_normal,
                    text_rep<'N'>
                >,
                basic_text_span<
                    text_style_above,
                    text_rep<'H'>
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
        //text_single<'O', 0, SINGLE_CHAR_OFFSET_Y>::draw(str, dbond, ctx.flip);
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

//struct alaine_svg_id { static constexpr int value{0}; };
//using alanine =
//    basic_amino_acid<
//        alaine_svg_id,
//        bond_type::plain,
//        basic_side_chain<
//            basic_side_chain_bond<bond_type::wedged>
//        >
//    >;

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
                    basic_text<
                        text_placement_down,
                        basic_text_span<text_style_normal, text_rep<'O'>>
                    >
                >,
                basic_side_chain<
                    basic_side_chain_bond<bond_type::plain, 0>,
                    basic_text<
                        text_placement_right,
                        basic_text_span<text_style_normal, text_rep<'N', 'H'>>,
                        basic_text_span<text_style_small_straight, text_rep<'2'>>
                    >
                >
            >
        >
    >;

//struct aspartate_svg_id { static constexpr int value{5}; };
//using aspartate =
//    basic_amino_acid<
//        aspartate_svg_id,
//        bond_type::dashed,
//        basic_side_chain<
//            basic_side_chain_bond<bond_type::plain>,
//            basic_side_chain_bond<bond_type::plain>,
//            basic_side_chain_split<
//                basic_side_chain<
//                    basic_side_chain_bond<bond_type::double_bond, 90>,
//                    text_single<'O', 0, (-(SINGLE_CHAR_OFFSET_Y)*2)/3>
//                >,
//                basic_side_chain<
//                    basic_side_chain_bond<bond_type::plain, 0>,
//                    text_single_with_smol<'O', '-', 0, 0>
//                >
//            >
//        >
//    >;
//
//struct cysteine_svg_id { static constexpr int value{1}; };
//using cysteine =
//    basic_amino_acid<
//        cysteine_svg_id,
//        bond_type::dashed,
//        basic_side_chain<
//            basic_side_chain_bond<bond_type::plain>,
//            basic_side_chain_bond<bond_type::plain>,
//            text_hor<'S', 'H', SINGLE_CHAR_OFFSET_X*2, 0>
//        >
//    >;
//
//struct glycine_svg_id { static constexpr int value{2}; };
//using glycine =
//    basic_amino_acid<
//        glycine_svg_id,
//        bond_type::plain,
//        basic_side_chain<>
//    >;

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
                    basic_text<
                        text_placement_down_left,
                        basic_text_span<
                            text_style_normal,
                            text_rep<'O'>
                        >
                    >
                >,
                basic_side_chain<
                    basic_side_chain_bond<bond_type::plain, 45>,
                    basic_text<
                        text_placement_down_right,
                        basic_text_span<
                            text_style_normal,
                            text_rep<'N', 'H'>
                        >,
                        basic_text_span<
                            text_style_small_angle,
                            text_rep<'2'>
                        >
                    >
                    //text_hor<'N', 'H', SINGLE_CHAR_OFFSET_X*2, -(SINGLE_CHAR_OFFSET_Y)/2>,
                    //text_single_smol<'2', 0, SINGLE_CHAR_OFFSET_Y>
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

    //append(alanine{});
    append(aspariqine{});
    //append(aspartate{});
    //append(cysteine{});
    //append(glycine{});
    append(glutamine{});

    draw_context ctx{};
    ctx.x -= SINGLE_CHAR_OFFSET_X*3.2;

    basic_text<
        text_placement_right,
        basic_text_span<
            text_style_normal,
            text_rep<'H'>
        >,
        basic_text_span<
            text_style_small_straight,
            text_rep<'2'>
        >
    >::draw(str, ctx, false);

    fixed_offset<SINGLE_CHAR_OFFSET_X*2, SINGLE_CHAR_OFFSET_Y>::draw(str, ctx, false);
    basic_text<
        text_placement_right,
        basic_text_span<
            text_style_normal,
            text_rep<'+'>
        >
    >::draw(str, ctx, false);
    //text_single<'H', SINGLE_CHAR_OFFSET_X, -(SINGLE_CHAR_OFFSET_Y*2)>::draw(str, ctx);
    //format(str, "<text text-anchor='left' dominant-baseline='middle' x='", ctx.x, "' y='", ctx.y + SINGLE_CHAR_OFFSET_Y*2.5, "' style='font-size:6px;'>2</text>");
    //text_single<'+', SINGLE_CHAR_OFFSET_X, SINGLE_CHAR_OFFSET_Y>::draw(str, ctx);

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
        ctx.x - DRAW_CTX_OFFSET_X + (TEXT_MARGIN*2) <<
        "' y='" <<
        ctx.y - DRAW_CTX_OFFSET_Y <<
        "' />\n";

    ctx.x += (std::cos(to_radians(-45+80)) * BOND_LENGTH * 3) + (SINGLE_CHAR_OFFSET_X * 2) + (TEXT_MARGIN*2); // single char offset appears to be a half char offset
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

    //draw(sstream, ctx, ctoaacai('D'));
    //draw(sstream, ctx, ctoaacai('D'));
    draw(sstream, ctx, ctoaacai('N'));
    draw(sstream, ctx, ctoaacai('N'));
    draw(sstream, ctx, ctoaacai('Q'));
    draw(sstream, ctx, ctoaacai('Q'));
    //draw(sstream, ctx, ctoaacai('C'));
    //draw(sstream, ctx, ctoaacai('C'));
    //draw(sstream, ctx, ctoaacai('A'));
    //draw(sstream, ctx, ctoaacai('G'));
    //draw(sstream, ctx, ctoaacai('G'));
    //draw(sstream, ctx, ctoaacai('A'));

    auto str = sstream.str();
    fixed_offset<static_cast<int>(TEXT_MARGIN*2), 0>::draw(str, ctx, false);
    basic_text<
        text_placement_right,
        basic_text_span<
            text_style_normal,
            text_rep<'O'>
        >,
        basic_text_span<
            text_style_high_nm,
            text_rep<'-'>
        >
    >::draw(str, ctx, false);
    //text_single<'O', SINGLE_CHAR_OFFSET_X, 0>::draw(str, ctx);
    //text_single<'-', SINGLE_CHAR_OFFSET_X, -SINGLE_CHAR_OFFSET_Y>::draw(str, ctx);

    //sstream << "</svg>\n";
    str.append("</svg>\n");

    std::printf("%s\n", str.c_str());

    return 0;
}

#pragma clang diagnostic pop