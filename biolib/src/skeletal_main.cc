#include <bio/skeletal/skeletal.hh>

#include <cstring>
#include <iostream>
#include <span>

int main(int argc, char** argv)
{
    std::span<char> span{argv[1], std::strlen(argv[1])}; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::cout << bio::draw_skeletal(span) << '\n';

    return 0;
}