#pragma once

#include <control.hpp>

#include <pspgum.h>

// TODO: have a look at sceGumLookAt()
namespace graphics
{
class Camera
{
private:
    ScePspFVector3 pos;
    ScePspFVector3 view;

    float fovy, znear, zfar;

public:
    Camera(float fovy, float znear, float zfar);
    ~Camera();

    void useView();
    void moveAlongView(ScePspFVector3& vec);
    void addRotation(ScePspFVector3& vec);
    void setRotation(ScePspFVector3& vec);

    ScePspFVector3 getPos() const;
    ScePspFVector3 getView() const;
};
} // namespace graphics