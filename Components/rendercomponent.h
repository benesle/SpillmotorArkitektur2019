#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include <QOpenGLFunctions_4_1_Core>


class MaterialComponent;
class MeshComponent;
class TransformComponent;
class CollisionComponent;
/**
 * The RenderComponent class
 * Contains the MaterialComponent, MeshComponent, TransformComponent and CollisionComponent.
 * Contains a funcyion to draw the collisionshape
 */
class RenderComponent : public QOpenGLFunctions_4_1_Core
{

public:
    /**
     * RenderComponent constructor that takes inn materialcomponent, meshcomponent, transformcomponent and collisioncomponent.
     */
    RenderComponent(MaterialComponent * mMat, MeshComponent* mMesh, TransformComponent* mTrans, CollisionComponent* mCol);
    //EntityID

    int entityID{0};
    void draw();
    /**
     * drawCollisionComponent
     * draws the collision component to the screen
     */
    void drawCollisionComponent();

private:
    MaterialComponent* mMaterialComp;
    MeshComponent* mMeshComp;
    TransformComponent* mTransComp;
    CollisionComponent* mCollisComp;

};

#endif // RENDERCOMPONENT_H
