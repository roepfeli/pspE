#pragma once

#include <stdint.h>
#include <string>

namespace graphics
{
struct Texture
{
    int width, height;
    uint16_t pow2height, pow2width;

    uint16_t bufferWidth, bufferHeight;

    int psm;

    void* ramData;
    void* vramData;

    Texture(const std::string& pathToFile);
    ~Texture();

    bool isInRam() const;
    bool isInVram() const;

    void removeFromVram();
    void removeFromRam();
};
} // namespace graphics