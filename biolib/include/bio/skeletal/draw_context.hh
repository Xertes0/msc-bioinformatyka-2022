#pragma once

namespace bio::skeletal
{

static constexpr double DRAW_CTX_OFFSET_X = 75;
static constexpr double DRAW_CTX_OFFSET_Y = 150;

struct
draw_context
{
    bool flip{false};
    double x{DRAW_CTX_OFFSET_X};
    double y{DRAW_CTX_OFFSET_Y};
};

} // namespace bio::skeletal