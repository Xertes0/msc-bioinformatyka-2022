#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma once

#include <string>

namespace bio::skeletal::detail
{

constexpr
void
format(std::string &str, double val)
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
format(std::string &str, char val)
{
    str.push_back(val);
}

template<std::size_t N>
constexpr
void
format(std::string &str,
       char const (&val)[N]) // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
{
    str.append(val);
}

constexpr
void
format(std::string &str, char const *val)
{
    str.append(val);
}

constexpr
void
format(std::string &str, std::string_view val)
{
    str.append(val);
}

template<class... Args>
constexpr
void
format(std::string &str, Args... args)
{
    (format(str, args), ...);
}

} // namespace bio::skeletal::detail
#pragma clang diagnostic pop