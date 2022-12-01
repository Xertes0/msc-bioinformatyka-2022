#include <bio/skeletal/skeletal.hh>

#include <cstring>
#include <iostream>
#include <span>

int main(int argc, char** argv)
{
    std::cout << "<svg id='amino_acid_svg' xmlns='http://www.w3.org/2000/svg'>\n";

    std::span<char> span{argv[1], std::strlen(argv[1])}; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::cout << bio::draw_skeletal(span) << '\n';

    std::cout << "</svg>\n";

    return 0;
}