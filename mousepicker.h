#ifndef MOUSEPICKER_H
#define MOUSEPICKER_H

#include "innpch.h"


class Matrix4x4;
class Vector3D;
class Camera;
class QMouseEvent;
class RenderWindow;
class Factory;
class EntityManager;

/**
 * The MousePicker class
 */
class MousePicker
{

public:
    //Camera already has a vewMat and proMat
    /**
     *  MousePicker
     * takes in Render and Camera
     */
    MousePicker(RenderWindow* mRender, Camera* cam);
    //Later make it take in Terrain
    gsl::Vector3D getCurrentRay();
    gsl::Vector3D getPointOnRay(float distance, gsl::Vector3D mRay);
    void update();
    void initMousePicker();

    float mouseX;
    float mouseY;


private:
    gsl::Vector3D calculateMouseRay();

    gsl::Vector3D mCurrentRay;
    gsl::Matrix4x4 mProjectionMatrix;
    gsl::Matrix4x4 mViewMatrix;

    Camera* mCamera;
    RenderWindow* mRenderWindow;
    gsl::Matrix4x4* mMatrix4x4;


    gsl::Vector2D getNormalizedDeviceCoords(/*float mouseX, float mouseY*/);
    gsl::Vector4D toEyeCoords(gsl::Vector4D clipCoorrds);
    gsl::Vector3D toWorldCoords(gsl::Vector4D eyeCoords);



};

#endif // MOUSEPICKER_H
