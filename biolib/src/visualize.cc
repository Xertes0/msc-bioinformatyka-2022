#include <string_view>
#include <ostream>
#include <cmath>
#include <numeric>

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
};

namespace svg
{

struct
draw_context
{
    double rot_{0};
    double x_{0};
    double y_{0};

    double length_{25};

    auto&
    bond(double rot, bond_type bond = bond_type::plain)
    {
        rot_ += rot;

        switch(bond) {
            case bond_type::plain:
            {
                std::printf("<line x1='%f' y1='%f' x2='%f' y2='%f' stroke='black' />\n", x_, y_,
                    x_ + (std::cos(to_radians(rot_))*length_),
                    y_ + (std::sin(to_radians(rot_))*length_)
                );
                break;
            }

            case bond_type::wedged:
            {

                auto old_xa = x_ + (std::cos(to_radians(rot_-90))*(length_*0.1));
                auto old_ya = y_ + (std::sin(to_radians(rot_-90))*(length_*0.1));

                auto old_xb = x_ + (std::cos(to_radians(rot_+90))*(length_*0.1));
                auto old_yb = y_ + (std::sin(to_radians(rot_+90))*(length_*0.1));

                std::printf("<polygon points='%f %f, %f %f, %f %f' fill='url(#bond-wedged)' />\n",
                    old_xa, old_ya,
                    old_xb, old_yb,
                    x_ + (std::cos(to_radians(rot_+rot))*length_),
                    y_ + (std::sin(to_radians(rot_+rot))*length_)
                );
                break;
            }

            default: throw std::runtime_error{"not implemented"};
        }

        x_ += (std::cos(to_radians(rot_))*length_);
        y_ += (std::sin(to_radians(rot_))*length_);

        return *this;
    }

    auto&
    line_u(double rot)
    {
        rot_ += rot;
        x_ += (std::cos(to_radians(rot_))*length_);
        y_ += (std::sin(to_radians(rot_))*length_);

        return *this;
    }

    auto&
    split(auto lamb)
    {
        lamb(draw_context{*this});
        return *this;
    }
};

}

void
draw_c()
{
    svg::draw_context ctx{};
    ctx.rot_ = 45;
    ctx.x_ = 50;
    ctx.y_ = 50;

    ctx
        .bond(0, bond_type::wedged)
        .split([](auto ctx) {
            ctx
                .bond(45)
                .bond(-45);
        })
        .bond(-90)
        .split([](auto ctx) {
            ctx
                .bond(-45);
        })
        .bond(90);

    //root2_x = root_x;
    //root2_y = root_y;
    //uu(root2_x, root2_y, lengths);

    //fd(root_x, root_y, length);
    //ctx.line(90);
}

int main()
{
    std::printf("<svg xmlns='http://www.w3.org/2000/svg'>\n");
    std::printf("<defs><pattern id='bond-wedged' height='10%%' width='10%%'><line x1='0' x2='0' y1='0' y2='10' stroke-width='1' stroke='black'></line></pattern></defs>\n");

    draw_c();

    std::printf("</svg>\n");

    return 0;
}
