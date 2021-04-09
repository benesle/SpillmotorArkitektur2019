#include "transformcomponent.h"

gsl::Matrix4x4 &TransformComponent::modelMatrix()
{
    return mModelMatrix;
}

gsl::Vector3D TransformComponent::getPosition()
{
    return translateMatrix.getPosition();
}

void TransformComponent::setModelMatrix(gsl::Matrix4x4 mat)
{
    mModelMatrix = mat;
}

void TransformComponent::translate(gsl::Vector3D trans)
{
    translateMatrix.translate(trans);
    mModelMatrix = translateMatrix * scaleMatrix;
}

void TransformComponent::scale(gsl::Vector3D scale)
{
    scaleMatrix.scale(scale);
    mModelMatrix = translateMatrix * scaleMatrix;
}

void TransformComponent::setPosition(gsl::Vector3D translater)
{
    translateMatrix.setToIdentity();
    translateMatrix.translate(translater);
    mModelMatrix = translateMatrix * scaleMatrix;
}

TransformComponent::TransformComponent()
{
    mModelMatrix.setToIdentity();
    translateMatrix.setToIdentity();
    scaleMatrix.setToIdentity();

}
