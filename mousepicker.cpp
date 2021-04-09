#include "mousepicker.h"
#include "vector3d.h"
#include "matrix4x4.h"
#include "camera.h"
#include "renderwindow.h"
#include "entitymanager.h"
#include "collisionmanager.h"
#include "collisioncomponent.h"
#include "meshcomponent.h"
#include "transformcomponent.h"


MousePicker::MousePicker(RenderWindow* mRender, Camera *cam)
{
    mRenderWindow = mRender;
    mCamera = cam;


}

gsl::Vector3D MousePicker::getCurrentRay()
{
    return mCurrentRay;
}

void MousePicker::update()
{
    initMousePicker();
    mCurrentRay = calculateMouseRay();
}

void MousePicker::initMousePicker()
{
    mCamera = mRenderWindow->getCurrentCamera();
    mProjectionMatrix = mCamera->mProjectionMatrix;
    mViewMatrix = mCamera->mViewMatrix;
}

gsl::Vector3D MousePicker::calculateMouseRay()
{
    mouseX = mRenderWindow->getMouseX();
    mouseY = mRenderWindow->getMouseY();
    gsl::Vector2D normalizedCoords = getNormalizedDeviceCoords(/*Already been set*/);
    gsl::Vector4D clipCoords = gsl::Vector4D(normalizedCoords.x, normalizedCoords.y, -1.f, -1.f);
    gsl::Vector4D eyeCoords = toEyeCoords(clipCoords);
    gsl::Vector3D worldRay = toWorldCoords(eyeCoords);

    return worldRay;
}

gsl::Vector3D MousePicker::getPointOnRay(float distance, gsl::Vector3D mRay)
{
    gsl::Vector3D camPos = mCamera->position();
    gsl::Vector3D startPos = camPos;
    gsl::Vector3D scaledRay = gsl::Vector3D(mRay.x * distance, mRay.y * distance, mRay.z * distance);
    return startPos + scaledRay;

}

gsl::Vector2D MousePicker::getNormalizedDeviceCoords()
{
    float x = (2.f*mouseX) / mRenderWindow->width() -1;
    float y = 1-(2.f*mouseY) / mRenderWindow->height();
    std::cout << "normalized Coords: " << "X: " << x << "Y: " << y << std::endl;

    return gsl::Vector2D(x,y);
}

gsl::Vector4D MousePicker::toEyeCoords(gsl::Vector4D clipCoorrds)
{
    gsl::Vector4D eyeCoords;
    gsl::Matrix4x4 mInvertedProjection = mProjectionMatrix;
    mInvertedProjection.inverse();
    eyeCoords = (mInvertedProjection* clipCoorrds);
    eyeCoords = gsl::Vector4D(eyeCoords.x, eyeCoords.y, -1.f, 0);
    return  eyeCoords;
}

gsl::Vector3D MousePicker::toWorldCoords(gsl::Vector4D eyeCoords)
{
    gsl::Matrix4x4 mInvertedViewMatrix = mViewMatrix;
    mViewMatrix.inverse();
    gsl::Vector4D rayWorld = (mInvertedViewMatrix* eyeCoords);
    rayWorld = gsl::Vector4D(rayWorld.x, rayWorld.y, rayWorld.z);
    rayWorld.normalize();
    return gsl::Vector3D(rayWorld.x, rayWorld.y, rayWorld.z);
}

