#include "movementsystem.h"
#include "entity.h"
#include "transformcomponent.h"
#include "coreengine.h"
#include "entitymanager.h"
#include "collisionmanager.h"
#include "collisioncomponent.h"
#include "meshcomponent.h"

MovementSystem::MovementSystem(CoreEngine* mCoreEngine )
{
    coreEngine = mCoreEngine;
}



void MovementSystem::enemyMove()
{

}

void MovementSystem::projectileMovement()
{
    std::vector<Entity* > tempBullet;
    tempBullet = coreEngine->getEntityManager()->getEntities();
    for(auto temp: tempBullet)
    {
        if(coreEngine->getEntityManager()->getEntities().at(1) != temp)
        {
            if(coreEngine->getCollisionManager()->checkCollision(coreEngine->getEntityManager()->getEntities().at(1)->getCollComp(), coreEngine->getEntityManager()->getEntities().at(1)->getTransComp(), temp->getCollComp(), temp->getTransComp()))
            {
                if(temp->typeEntity == Entity::ENEMY )
                {
                    temp->getMeshComp()->isRendering = false;
                    temp->getCollComp()->isColliding = false;
                    coreEngine->getEntityManager()->getEntities().at(1)->getTransComp()->translate(gsl::Vector3D{100,0,-0.1});
                }
            }
            else
            {

            }
            coreEngine->getEntityManager()->getEntities().at(1)->getTransComp()->translate(gsl::Vector3D{0,0,-0.1});
        }
    }
}

void MovementSystem::update()
{
    if(coreEngine->isRunning)
    {
        enemyMove();
        projectileMovement();
    }
    else
    {
        //Generell move
        move();
    }
}

    void MovementSystem::move()
    {
        //Move for editor
    }

