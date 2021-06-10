#include <Camera.hpp>

#include <pspgu.h>
#include <pspgum.h>

namespace graphics
{
Camera::Camera(float fovy, float znear, float zfar)
    : fovy(fovy)
    , znear(znear)
    , zfar(zfar)
{
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;

    view.x = 0.0f;
    view.y = 0.0f;
    view.z = 0.0f;
}

Camera::~Camera() { }

void Camera::useView()
{
    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();
    sceGumRotateXYZ(&view);
    sceGumTranslate(&pos);

    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();
    sceGumPerspective(fovy, 16.0f / 9.0f, znear, zfar);
}

void Camera::moveAlongView(ScePspFVector3& vec)
{
    ScePspFMatrix4 viewMatrix;

    sceGumMatrixMode(GU_VIEW);
    sceGumStoreMatrix(&viewMatrix);
    //sceGumPopMatrix();

    pos.x += viewMatrix.x.x * vec.x + viewMatrix.x.y * vec.y + viewMatrix.x.z * vec.z;
    pos.y += viewMatrix.y.x * vec.x + viewMatrix.y.y * vec.y + viewMatrix.y.z * vec.z;
    pos.z += viewMatrix.z.x * vec.x + viewMatrix.z.y * vec.y + viewMatrix.z.z * vec.z;
}

void Camera::addRotation(ScePspFVector3& vec)
{
    view.x += vec.x;
    view.y += vec.y;
    view.z += vec.z;
}

void Camera::setRotation(ScePspFVector3& vec)
{
    view.x = vec.x;
    view.y = vec.y;
    view.z = vec.z;
}

ScePspFVector3 Camera::getPos() const
{
    return pos;
}

ScePspFVector3 Camera::getView() const
{
    return view;
}

} // namespace graphics