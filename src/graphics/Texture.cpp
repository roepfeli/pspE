#include <Texture.hpp>

#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <pspgu.h>
#include <pspkernel.h>

#include <pspdebug.h>

#include <vram.h>

// TODO: have a look at sceGuCopyImage()

static int numAlign(int n, int numBits)
{
    return (n % numBits == 0) ? n : n - (numBits - (n % numBits));
}

static int nextPow2(int n)
{
    if (n <= 0)
    {
        return 0;
    }

    int pow2 = 1;

    while (pow2 < n)
    {
        pow2 <<= 1;
    }

    return pow2;
}

namespace graphics
{

Texture::Texture(const std::string& pathToFile)
{
    vramData = ramData = NULL;

    uint16_t pWidth, pHeight;

    //stbi_set_flip_vertically_on_load(1);
    //this->ramData = stbi_load(pathToFile.c_str(), &pWidth, &pHeight, &bpp, 4);
    SceUID fp = sceIoOpen(pathToFile.c_str(), PSP_O_RDONLY, 0777);

    // read in dimensions:
    sceIoRead(fp, &pWidth, sizeof(uint16_t));
    sceIoRead(fp, &pHeight, sizeof(uint16_t));

    // read in pixels:
    size_t pictureSize = sizeof(uint32_t) * pWidth * pHeight;

    ramData = malloc(pictureSize);
    vramData = valloc(pictureSize);

    sceIoRead(fp, ramData, pictureSize);

    memcpy(vramData, ramData, pictureSize);

    this->width = pWidth;
    this->height = pHeight;

    this->psm = GU_PSM_8888;

    this->bufferWidth = nextPow2(pWidth);
    this->bufferHeight = numAlign(pWidth, 8);

    this->pow2height = nextPow2(pHeight);
    this->pow2width = nextPow2(pWidth);

    sceKernelDcacheWritebackAll();
}

Texture::~Texture()
{
    if (ramData)
    {
        free(ramData);
    }

    if (vramData)
    {
        vfree(vramData);
    }
}

bool Texture::isInRam() const
{
    return ramData;
}

bool Texture::isInVram() const
{
    return vramData;
}

void Texture::removeFromVram()
{
    vfree(vramData);
    vramData = NULL;
}

void Texture::removeFromRam()
{
    free(ramData);
    ramData = NULL;
}

} // namespace graphics