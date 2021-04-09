#ifndef ENTITY_H
#define ENTITY_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>


class RenderWindow;
class MeshComponent;
class MaterialComponent;
class TransformComponent;
class RenderComponent;
class CollisionComponent;
class Entity
{
public:
    /**
     * Entity class
     * contains an enum class with entity types and getters for meshcomponent, materialcomponent, rendercomponent and collisioncomponent
     */
    Entity();
    ~Entity();
    /**
     * The entityType enum
     * Contains etity types such as Player, Enemy, Bullet, StaticObject, CollisionBound, Camera, Light, Sprite and Hud
     */
    enum entityType
    {
        PLAYER,
        ENEMY,
        BULLET,
        STATICOBJECT,
        COLLISIONBOUND,
        CAMERA,
        LIGHT,
        SPRITE,
        HUD
    };

    entityType typeEntity = STATICOBJECT;


    MeshComponent *getMeshComp();
    MaterialComponent* getMatComp();
    TransformComponent* getTransComp();
    RenderComponent* getRenderComp();
    CollisionComponent* getCollComp();

    unsigned int entityID;
    std::string entityName{""};

    Entity *mParent{nullptr};
    Entity *mChild{nullptr};

    std::vector<Entity*> mChildren;

    void add(Entity* mChild)
    {
        mChild->mParent = this;
        mChildren.push_back(mChild);
    }

    void setEntityID(unsigned int inEntityID);
    void setEntityName(std::string nameIn);

    unsigned int getEntityID();
    std::string getEntityName();

    void attachChild(Entity* childEntity);
    Entity* getChild();
    Entity* getParent();
    bool hasChild();
    bool hasParent();

    void setMeshComp(MeshComponent *meshComp);
    void setTransComp(TransformComponent* transComp);
    void setMatComp(MaterialComponent *matComp);
    void setColComp(CollisionComponent *collComp);
    void setRenderComp();

private:

    MeshComponent *mMeshComp{nullptr};
    MaterialComponent *mMatComp{nullptr};
    TransformComponent* mTransComp{nullptr};
    RenderComponent* mRenderComp{nullptr};
    CollisionComponent* mCollComp{nullptr};


};

#endif // ENTITY_H
