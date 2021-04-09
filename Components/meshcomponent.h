#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include "innpch.h"
/**
 * The MeshComponent class
 * contains a string with name, and other variables such as mVertices, mIndices, mVAO, mVBO, mEAB, EntityID
 */
class MeshComponent
{
public:
    MeshComponent();

    std::string mName;
    std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mIndices;    //This is usually not needed after object is made

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

    bool isRendering = true;
    int EntityID;

    unsigned long long mVerticesSize()
    {
        return mVertices.size();
    }
    unsigned long long mIdicesSize()
    {
        return mIndices.size();
    }

    //std::string nName
    //mVertecis
    //mIndices
    //VAO
    //VBO
    //EAB
    //mVertecisSize
    //mIndicesSize
    //entity -entityID



    ~MeshComponent();
};

#endif // MESHCOMPONENT_H
