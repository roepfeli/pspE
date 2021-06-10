#include <graphics.hpp>

#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psputils.h>
#include <vram.h>

static void* s_drawBuffer;
static void* s_displayBuffer;
static void* s_depthBuffer;

static unsigned int __attribute__((aligned(16))) displayList[262144];

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

namespace graphics
{

void init()
{
    s_drawBuffer = vrelptr(valloc(BUF_WIDTH * SCR_HEIGHT * 4));
    s_displayBuffer = vrelptr(valloc(BUF_WIDTH * SCR_HEIGHT * 4));
    s_depthBuffer = vrelptr(valloc(BUF_WIDTH * SCR_HEIGHT * 2));

    sceGuInit();

    sceGuStart(GU_DIRECT, displayList);

    sceGuDrawBuffer(GU_PSM_8888, s_drawBuffer, BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, s_displayBuffer, BUF_WIDTH);
    sceGuDepthBuffer(s_depthBuffer, BUF_WIDTH);

    sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
    sceGuDepthRange(65535, 0);
    // sceGuDepthRange(0xc350, 0x2710);

    // TODO: see if you can remove these lines of code
    sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);

    // TODO: see if you can remove these lines of code
    sceGuAlphaFunc(GU_GREATER, 0, 0xff);
    sceGuEnable(GU_ALPHA_TEST);

    sceGuDepthFunc(GU_GREATER);
    sceGuEnable(GU_DEPTH_TEST);

    sceGuFrontFace(GU_CCW);
    sceGuEnable(GU_CULL_FACE);

    sceGuEnable(GU_TEXTURE_2D);

    // TODO: play with the shader-options
    sceGuShadeModel(GU_SMOOTH);

    sceGuClearColor(0xff554433);
    // sceGuClearDepth(0xffff);
    sceGuClearDepth(0);

    // TODO: play with this
    // sceGuTexFilter(GU_LINEAR, GU_LINEAR);
    sceGuTexFilter(GU_NEAREST, GU_NEAREST);
    sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
    sceGuTexEnvColor(0xffffffff);
    sceGuColor(0xffffffff);
    sceGuAmbientColor(0xffffffff);
    sceGuTexOffset(0.0f, 0.0f);
    sceGuTexScale(1.0f, 1.0f);

    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);
}

void destroy()
{
    sceGuTerm();
}

void start()
{
    sceGuStart(GU_DIRECT, displayList);
}
void stop()
{
    sceGuFinish();
}

void clearScreen()
{
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
}

void draw3D(const Mesh& mesh, const Texture& texture)
{
    sceGuTexMode(texture.psm, 0, 0, 0);

    sceGuTexImage(0,
                  texture.pow2width,
                  texture.pow2height,
                  texture.bufferWidth,
                  (texture.vramData) ? texture.vramData : texture.ramData);

    sceGumDrawArray(GU_TRIANGLES,
                    GU_TYPE_VERTEX_3D,
                    mesh.getNumOfDrawVertices(),
                    mesh.getIndiciesAddr(),
                    mesh.getVerticiesAddr());
}

void disableFog()
{
    sceGuDisable(GU_FOG);
}

void useFog(float near, float far, unsigned int color)
{
    sceGuFog(near, far, color);
    sceGuEnable(GU_FOG);
}

void draw2D(const Sprite& sprite)
{
    if (sprite.texture != NULL)
    {
        sceGuTexMode(sprite.texture->psm, 0, 0, 0);

        sceGuTexImage(0,
                      sprite.texture->pow2width,
                      sprite.texture->pow2height,
                      sprite.texture->bufferWidth,
                      (sprite.texture->vramData) ? sprite.texture->vramData
                                                 : sprite.texture->ramData);
    }

    Vertex2D* tmp = (Vertex2D*)sceGuGetMemory(2 * sizeof(Vertex2D));

    tmp[0].x = sprite.x;
    tmp[0].y = sprite.y;
    tmp[0].z = 100000.0f;
    tmp[0].u = sprite.u;
    tmp[0].v = sprite.v;

    tmp[1].x = sprite.x + sprite.w;
    tmp[1].y = sprite.y + sprite.h;
    tmp[1].z = 100000.0f;
    tmp[1].u = sprite.u + sprite.u_w;
    tmp[1].v = sprite.v + sprite.v_h;

    sceGuDrawArray(GU_SPRITES, GU_TYPE_VERTEX_2D, 2, NULL, (void*)tmp);
}

// TODO: this is retarded implement a batch-renderer
void draw2D(const std::vector<Sprite>& sprites)
{
    for (const auto sprite : sprites)
    {
        draw2D(sprite);
    }
}

void present()
{
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

} // namespace graphics