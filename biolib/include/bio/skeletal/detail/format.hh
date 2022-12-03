#pragma once

#include <string>

namespace bio::skeletal::detail
{

static constexpr auto FORMAT_MAX_DOUBLE_DIGITS = 7U;

constexpr
void
format_s(std::string &str, double val)
{
    int nat = static_cast<int>(val);
    int fra = static_cast<int>((val - nat) * (1U << FORMAT_MAX_DOUBLE_DIGITS));
    std::string buf{"."};

    while(nat != 0) {
        buf.push_back(static_cast<char>('0' + (nat % 10)));
        nat /= 10;
    }
    if(buf.ends_with('.')) {
        buf.push_back('0');
    }
    str.append(std::string{buf.rbegin(), buf.rend()});

    buf.clear();

    while(fra != 0) {
        buf.push_back(static_cast<char>('0' + (fra % 10)));
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
format_s(std::string &str, char val)
{
    str.push_back(val);
}

template<std::size_t N>
constexpr
void
format_s(std::string &str, char const (&val)[N]) // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
{
    str.append(val);
}

constexpr
void
format_s(std::string &str, char const *val)
{
    str.append(val);
}

constexpr
void
format_s(std::string &str, std::string_view val)
{
    str.append(val);
}

template<class... Args>
constexpr
void
format(std::string &str, Args... args)
{
    (format_s(str, args), ...);
}

} // namespace bio::skeletal::detail