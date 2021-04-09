#include "innpch.h"
#include "entitymanager.h"
#include "factory.h"
#include "entity.h"
#include "renderwindow.h"
#include "mainwindow.h"
#include "meshcomponent.h"
#include "materialcomponent.h"
#include "transformcomponent.h"
#include "collisioncomponent.h"
#include "rendercomponent.h"


EntityManager::EntityManager(MainWindow * mainwindow, RenderWindow *renderwindow, Shader* shader)
{
    shaderProgram[0] = &shader[0];
    shaderProgram[1] = &shader[1];
    shaderProgram[2] = &shader[2];
    shaderProgram[3] = &shader[3];

    mFactory = new Factory;

    mMainWindow  = mainwindow;
    mRenderWindow = renderwindow;
    //Lage factory med new
    //sette mMainWindow
    // sette mRenderWindow
    makeScene();
}

EntityManager::~EntityManager()
{
    delete mFactory;

    //Slette pekere som du har brukt "new" på i denne klassen.
}

void EntityManager::makeScene()
{
    makePlayer("box2.txt", gsl::Vector3D{6,0,40},0, "BoundingSphere");
    makeProjectile("ball0", getEntities().at(entityCount-1)->getTransComp()->modelMatrix().getPosition(), 0, "BoundingSphere" );
    getEntities().at(entityCount-1)->getTransComp()->translate(gsl::Vector3D{100,0, 1});
    getEntities().at(entityCount-1)->getTransComp()->scale(gsl::Vector3D{0.05f, 0.05f, 0.05f});
//    getEntities().at(entityCount-1)->getTransComp()->modelMatrix().translate(0,0,1);
//    getEntities().at(entityCount-1)->getTransComp()->modelMatrix().scale(0.05f);
    spawnEnemis();



}

void EntityManager::makeEnemy(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume)
{
    Entity* mEntity = new Entity;
    mEntity->typeEntity = Entity::ENEMY;
    mEntity->setEntityID(entityCount);
    mEntity->setMeshComp(mFactory->makeMeshComponent(meshFilename, entityCount));
    std::cout << mEntity->getMeshComp()->mVAO << "  MeshCOMP VAO" << std::endl;
    mEntity->setMatComp(mFactory->makeMaterialComponent(shaderProgram[shaderIndex], entityCount));
    mEntity->setTransComp(mFactory->makeTransformComponent(position, entityCount));
    mEntity->setColComp(mFactory->makeCollisionComponent(collisionVolume, entityCount) );
    mEntity->setRenderComp();
    std::cout << "EnemyType: " << mEntity->typeEntity << std::endl;
    mEntities.push_back(mEntity);
    entityCount++;

}


void EntityManager::spawnEnemis()
{
    int numberOfenemies = 10;
    gsl::Vector3D enemyPosition{0.5,0, 5};
    gsl::Vector3D startPosition = enemyPosition;
    for(int i = 0; i < 5; i++ )
    {
        enemyPosition.z += 2;

        for(int j = 0; j < 5; j++)
        {

            enemyPosition.x += 2;

            makeEnemy("monkey.obj", enemyPosition, 0, "BoundingSphere");
        }

        enemyPosition.x = startPosition.x;
    }

}

void EntityManager::reset()
{
    for(auto ent: mEntities)
    {
        ent->getMeshComp()->isRendering = true;
        ent->getCollComp()->isColliding = true;
    }
    mEntities.at(0)->getTransComp()->setPosition(gsl::Vector3D{6,0,40});
}

void EntityManager::makeEntity(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume)
{
    Entity* mEntity = new Entity;
    mEntity->setEntityID(entityCount);
    mEntity->setMeshComp(mFactory->makeMeshComponent(meshFilename, entityCount));
    std::cout << mEntity->getMeshComp()->mVAO << "  MeshCOMP VAO" << std::endl;
    mEntity->setMatComp(mFactory->makeMaterialComponent(shaderProgram[shaderIndex], entityCount));
    mEntity->setTransComp(mFactory->makeTransformComponent(position, entityCount));
    mEntity->setColComp(mFactory->makeCollisionComponent(collisionVolume, entityCount) );
    mEntity->setRenderComp();
    mEntities.push_back(mEntity);
    entityCount++;
}

void EntityManager::makePlayer(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume)
{
    makeEntity(meshFilename, position, shaderIndex, collisionVolume);
    mEntities.at(entityCount - 1)->typeEntity = Entity::PLAYER;
}

void EntityManager::makeProjectile(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume)
{
    makeEntity(meshFilename, position, shaderIndex, collisionVolume);
    mEntities.at(entityCount - 1)->typeEntity = Entity::BULLET;
}

void EntityManager::move(unsigned int ID)
{
    //Move Entity, by selected(ID)
    //MAKE LATER TranselationComponent
}

void EntityManager::render(unsigned int ID)
{
    Entity* tempEnt = findEntityByID(ID);

    if(tempEnt != nullptr)
    {
        RenderComponent* rendTemp =
                tempEnt->getRenderComp();

        if(rendTemp != nullptr)
        {
            rendTemp->draw();
            rendTemp->drawCollisionComponent();

        }
    }
    //->draw();
    //FindEntityBy ID,
    //Entity get rendercomponent
    // Entity draw
}
Entity *EntityManager::findEntityByID(unsigned ID)
{
    for(int i = 0; i < mEntities.size(); i++)
    {
        if (ID == mEntities.at(i)->entityID)
        {
            return mEntities.at(i);
        }
    }
    return nullptr;
}
std::vector<Entity *> EntityManager::getEntities() const
{
    return mEntities;
}

