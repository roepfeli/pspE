#pragma once

#include <Mesh.hpp>
#include <Sprite.h>
#include <Texture.hpp>
#include <Vertex.h>

#include <pspgum.h>

#include <stdint.h>

namespace graphics
{

void init();

void start();
void stop();

void destroy();

void draw3D(const Mesh& mesh, const Texture& texture);

void draw2D(const Sprite& sprite);
void draw2D(const std::vector<Sprite>& sprites);

void disableFog();

void useFog(float near, float far, unsigned int color);

void clearScreen();

void present();

} // namespace graphics