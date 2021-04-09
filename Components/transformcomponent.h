#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "innpch.h"

/**
 * The TransformComponent class
 * Contains Matrix4x4 and a function to set the model matrix
 */
class TransformComponent
{
    gsl::Matrix4x4 mModelMatrix;
    gsl::Matrix4x4 translateMatrix;
    gsl::Matrix4x4 scaleMatrix;

public:
    TransformComponent();

    //Movement
    gsl::Matrix4x4 &modelMatrix(); //please remove this, works just like mModelMatrix variable;
    gsl::Vector3D getPosition();
    /**
     *  setModelMatrix
     */
    void setModelMatrix(gsl::Matrix4x4);
    /**
     *  translate
     */
    void translate(gsl::Vector3D);
    /**
     * scale
     */
    void scale(gsl::Vector3D);
    /**
     *  setPosition
     */
    void setPosition(gsl::Vector3D);
    int entityID = 0;
};

#endif // TRANSFORMCOMPONENT_H
