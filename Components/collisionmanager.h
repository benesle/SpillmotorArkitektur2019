#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "innpch.h"

class CollisionComponent;
class TransformComponent;
/**
 *The CollisionManager class
 * Takes in collisioncomponent and transformcomponent
 * Handles the collision
 */
class CollisionManager
{
public:
    CollisionManager();

    bool checkCollision(CollisionComponent* tempColl1,TransformComponent* transComp1, CollisionComponent* tempColl2, TransformComponent* transComp2);
    bool sphereToSphere(CollisionComponent *sphere1, TransformComponent* transComp1, CollisionComponent *sphere2, TransformComponent* transComp2);
    bool pointToSphere(CollisionComponent *sphere0, TransformComponent* transComp0, const gsl::Vector3D& vec3P);

    bool pointInAABB( CollisionComponent* box, const gsl::Vector3D& vecP);
    bool AABBtoAABB( CollisionComponent* box1, CollisionComponent* box2);
};

#endif // COLLISIONMANAGER_H
