#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H

#include "material.h"
#include "shader.h"
/**
 * The MaterialComponent class
 */
class MaterialComponent
{
public:
    MaterialComponent();
    int entityID;
    /**
     *  setShader is a function to set the shader
     */
    void setShader(Shader *shader);

    Material &returnMaterial()
    {
        return mMaterial;
    }


private:
        Material mMaterial;
};

#endif // MATERIALCOMPONENT_H
