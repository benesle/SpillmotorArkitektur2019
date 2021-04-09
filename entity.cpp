#include "innpch.h"
#include "entity.h"
#include "meshcomponent.h"
#include "materialcomponent.h"
#include "transformcomponent.h"
#include "collisioncomponent.h"
#include "rendercomponent.h"
#include "shader.h"

Entity::Entity()
{

}

Entity::~Entity()
{

    for (auto mChild : mChildren)
    {
        delete mChild;
    }
    mChildren.clear();
}

MeshComponent* Entity::getMeshComp()
{
 return mMeshComp;
}

MaterialComponent* Entity::getMatComp()
{
    return mMatComp;
}

TransformComponent* Entity::getTransComp()
{
    return mTransComp;
}

RenderComponent *Entity::getRenderComp()
{
    return mRenderComp;
}

CollisionComponent *Entity::getCollComp()
{
    return mCollComp;
}

std::string Entity::getEntityName()
{
    return entityName;
}

unsigned int Entity::getEntityID()
{
    return entityID;
}

void Entity::attachChild(Entity *childEntity)
{
    mChild =childEntity;
    childEntity->mParent = this;
}

Entity *Entity::getChild()
{
    if (mChild != nullptr)
        {
            return mChild;
        }
        else
        {
            return nullptr;
        }
}

Entity *Entity::getParent()
{
    if (mParent != nullptr)
       {
           return mParent;
       }
       else
       {
           return nullptr;
       }

}

bool Entity::hasChild()
{

    if (mChild != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Entity::hasParent()
{
    if (mParent != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Entity::setMeshComp(MeshComponent *meshComp)
{
    mMeshComp = meshComp;
    if( meshComp->mName != "")
    {
    entityName = meshComp->mName;
    }
    else
    {
        entityName = "Entity_" + std::to_string(entityID);
    }
}

void Entity::setMatComp(MaterialComponent *matComp)
{
    mMatComp = matComp;
}

void Entity::setColComp(CollisionComponent *collComp)
{
    mCollComp = collComp;
}

void Entity::setRenderComp()
{
    mRenderComp = new RenderComponent(mMatComp ,mMeshComp, mTransComp, mCollComp);
    mRenderComp->entityID = entityID;
}

void Entity::setTransComp(TransformComponent *transComp)
{
    mTransComp = transComp;
}

void Entity::setEntityName(std::string nameIn)
{
    entityName = nameIn;
}

void Entity::setEntityID(unsigned int inEntityID)
{
    entityID = inEntityID;
    if(entityName == "")
    {
        setEntityName("Entity" + std::to_string(entityID));
    }
}


