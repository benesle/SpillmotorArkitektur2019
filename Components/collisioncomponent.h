#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H
#include "innpch.h"
class MeshComponent;
class QOpenGlFuctions;
class TransformComponent;
/**
     * The CollisionShape enum, choose which collision shape to use
     * AABB or BoundingSphere
     */
enum CollisionShape
{
    AABB,
    BoundingSphere

};

/** Collision komponent
     * This is the collision component class
     * Contains variables such as mName, mVertices, mIndices.
     */
class CollisionComponent
{
    //    Minimum coordinates in X, Y and Z
    //    Maximum coordinates in X, Y and Z

    //   Make fit the object: max-min in X, Y and Z
    //    Center of the object: (min+max)/2 in X, Y and Z
    //    Make cube around the object:

    //    Cube (1x1x1), centered
    //    We scale it to fit the object
    //    Center it on the object
    //    Draw the cube:
public:
    CollisionComponent();
    ~CollisionComponent();

    std::string mName;
    std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mIndices;    //This is usually not needed after object is made

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

    int EntityID;
    bool isVisible{false};
    bool isColliding = true;
    unsigned long long mVerticesSize()
    {
        return mVertices.size();
    }
    unsigned long long mIdicesSize()
    {
        return mIndices.size();
    }
    /**
* Initialized default CollisionShape
*/
    CollisionShape collShape {CollisionShape::BoundingSphere};

    gsl::Vector3D mMin;
    gsl::Vector3D mMax;
    gsl::Vector3D mCenter;
    float mRadius = 1.f;

};

#endif // COLLISIONCOMPONENT_H
