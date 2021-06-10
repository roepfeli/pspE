#pragma once

#include <Texture.hpp>

namespace graphics
{
struct Sprite
{
    size_t x, y;
    size_t w, h;

    size_t u, v;
    size_t u_w, v_h;

    Texture* texture;

    Sprite(Texture* texture,
           size_t x,
           size_t y,
           size_t w,
           size_t h,
           size_t u,
           size_t v,
           size_t u_w,
           size_t v_h)
        : x(x)
        , y(y)
        , w(w)
        , h(h)
        , u(u)
        , v(v)
        , u_w(u_w)
        , v_h(v_h)
    { }
};

} // namespace graphics