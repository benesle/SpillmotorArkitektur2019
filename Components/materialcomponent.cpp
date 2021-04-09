#include "materialcomponent.h"


MaterialComponent::MaterialComponent()
{

}

void MaterialComponent::setShader(Shader *shader)
{
    mMaterial.setShader(shader);
}
