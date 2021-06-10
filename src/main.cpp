#include <math.h>

#include <Camera.hpp>
#include <callback.hpp>
#include <control.hpp>
#include <graphics.hpp>

#include <pspgu.h>
#include <pspgum.h>

#include <pspkernel.h>

PSP_MODULE_INFO("snake", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void)
{
    util::setupCallbacks();

    graphics::init();

    graphics::start();
    graphics::useFog(0.5f, 40.0f, 0xff33aabb);
    graphics::stop();

    ctrl::init();

    ctrl::Pad pad;

    ScePspFVector3 pos;
    ScePspFVector3 view;

    graphics::Camera camera(75.0f, 0.5f, 1000.0f);

    graphics::Mesh cubeMesh("./res/objects/cube.obj");
    graphics::Texture defaultTexture("./res/textures/default.tex");

    graphics::Sprite sprite(&defaultTexture, 0, 0, 100, 100, 0, 0, 100, 100);

    int theta = 0;

    while (true)
    {
        ctrl::poll(&pad);

        pos.x = -(float)(pad.Lx - 128) / 128.0f * 0.2f;
        pos.z = -(float)(pad.Ly - 128) / 128.0f * 0.2f;

        if (pad.Buttons & PSP_CTRL_TRIANGLE)
        {
            view.x = -0.04f;
        }
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            view.x = 0.04f;
        }
        if (pad.Buttons & PSP_CTRL_SQUARE)
        {
            view.y = -0.04f;
        }
        if (pad.Buttons & PSP_CTRL_CIRCLE)
        {
            view.y = 0.04f;
        }

        camera.addRotation(view);
        camera.moveAlongView(pos);

        pos.x = pos.y = pos.z = 0.0f;
        view.x = view.y = view.z = 0.0f;

        graphics::start();

        graphics::clearScreen();

        camera.useView();

        for (size_t z = 0; z < 5; ++z)
            for (size_t y = 0; y < 5; ++y)
                for (size_t x = 0; x < 5; ++x)
                {
                    ScePspFVector3 tmp = {10.0f * x, 10.0f * y, 10.0f * z};

                    sceGumMatrixMode(GU_MODEL);
                    sceGumLoadIdentity();
                    sceGumTranslate(&tmp);
                    // {
                    //     ScePspFVector3 pos = {0, 0, -5.5f};
                    //     ScePspFVector3 rot = {theta * 0.79f * (GU_PI / 180.0f),
                    //                           theta * 0.98f * (GU_PI / 180.0f),
                    //                           theta * 1.32f * (GU_PI / 180.0f)};
                    //     sceGumTranslate(&pos);
                    //     sceGumRotateXYZ(&rot);
                    // }

                    graphics::draw3D(cubeMesh, defaultTexture);
                }

        graphics::draw2D(sprite);

        graphics::stop();

        graphics::present();

        ++theta;
    }

    graphics::destroy();

    sceKernelExitGame();
    return 0;
}