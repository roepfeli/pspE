#pragma once

#define GU_TYPE_VERTEX_3D (GU_TEXTURE_32BITF | GU_NORMAL_32BITF | GU_VERTEX_32BITF | GU_INDEX_16BIT)
#define GU_TYPE_VERTEX_2D (GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D)

#include <stdint.h>

namespace graphics
{

struct Vertex3D
{
    float u, v;
    float nx, ny, nz;
    float x, y, z;
};

struct Vertex2D
{
    uint16_t x, y, z;
    uint16_t u, v;

    uint16_t padd; // vertices need to align to 32bit, if not it must be padded
};

} // namespace graphics