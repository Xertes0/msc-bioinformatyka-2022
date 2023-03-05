#pragma once

#include <string_view>

namespace bio::skeletal
{

struct
text_style_normal
{
    static
    constexpr
    std::string_view value{};
};

struct
text_style_small
{
    static
    constexpr
    std::string_view value{"style='font-size:6px;' dy='0.575em'"}; //dominant-baseline='mathematical'
};

struct
text_style_below
{
    static
    constexpr
    std::string_view value{"dx='-0.6em' dy='1em'"};
};

struct
text_style_above
{
    static
    constexpr
    std::string_view value{"dx='-0.6em' dy='-1em'"};
};

struct
text_style_high_nm
{
    static
    constexpr
    std::string_view value{"dy='-0.6em'"};
};

template<class Style>
struct
flip_style;

template<>
struct
flip_style<text_style_high_nm>
{
    static
    constexpr
    std::string_view value{"dy='0.6em'"};
};

template<>
struct
flip_style<text_style_small>
{
    static
    constexpr
    std::string_view value{"dy='-0.2875em'"};
};

} // namespace bio::skeletal
