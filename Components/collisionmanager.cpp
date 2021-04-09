#include "collisionmanager.h"
#include "collisioncomponent.h"
#include "transformcomponent.h"


CollisionManager::CollisionManager()
{

}

bool CollisionManager::checkCollision(CollisionComponent *tempColl1, TransformComponent* transComp1, CollisionComponent *tempColl2, TransformComponent* transComp2)
{
    if(tempColl1 != nullptr && tempColl2 != nullptr)
    {
        if(tempColl1->isColliding == true && tempColl2->isColliding == true)
        {
            if(tempColl1->collShape == CollisionShape::BoundingSphere && tempColl2->collShape == CollisionShape::BoundingSphere)
            {
                return sphereToSphere(tempColl1,transComp1, tempColl2, transComp2);
            }
        }
    }
    return false;
}

bool CollisionManager::sphereToSphere(CollisionComponent* sphere1, TransformComponent *transComp1,   CollisionComponent* sphere2, TransformComponent *transComp2)
{
    gsl::Vector3D distance(transComp2->modelMatrix().getPosition() - transComp1->modelMatrix().getPosition());
    float distanceSquared(gsl::Vector3D::dot(distance, distance));

    float radiiSumSquared( sphere1->mRadius + sphere2->mRadius);
    radiiSumSquared *= radiiSumSquared;

    if(distanceSquared <= radiiSumSquared)
    {
        return true;
    }

    return false;
}

bool CollisionManager::pointToSphere(CollisionComponent *sphere0, TransformComponent *transComp0, const gsl::Vector3D &vec3P)
{
    gsl::Vector3D distance(transComp0->modelMatrix().getPosition() - vec3P);
    float distanceSquared(gsl::Vector3D::dot(distance, distance));
    float radiiSumSquared( sphere0->mRadius);
    radiiSumSquared *= radiiSumSquared;
    if(distanceSquared <= radiiSumSquared)
    {
        return true;
    }
    return false;
}

bool CollisionManager::pointInAABB(CollisionComponent* box, const gsl::Vector3D& vecP)
{
    if(vecP.x > box->mMin.x && vecP.x < box->mMax.x &&
            vecP.y > box->mMin.y && vecP.y < box->mMax.y &&
            vecP.z > box->mMin.z && vecP.z < box->mMax.z)
    {
        return true;
    }
    return false;
}

bool CollisionManager::AABBtoAABB(CollisionComponent *box1, CollisionComponent *box2)
{
    if(box1->mMax.x > box2->mMin.x &&
            box1->mMin.x < box2->mMax.x &&
            box1->mMax.y > box2->mMin.y &&
            box1->mMin.y < box2->mMax.y &&
            box1->mMax.z > box2->mMin.z &&
            box1->mMin.z < box2->mMax.z)
    {
        return true;
    }
    return false;
}
