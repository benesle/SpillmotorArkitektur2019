#include "innpch.h"
#include "coreengine.h"
#include "mainwindow.h"
#include "renderwindow.h"
#include "entitymanager.h"
#include "entity.h"
#include "collisionmanager.h"
#include "shader.h"
#include "mousepicker.h"
#include "movementsystem.h"
#include <QApplication>
#include <QCoreApplication>
#include <QObject>


EntityManager *CoreEngine::getEntityManager() const
{
    return mEntityManager;
}

CollisionManager *CoreEngine::getCollisionManager()
{
    return mCollisionManager;
}

CoreEngine::CoreEngine(MainWindow* mainWindow, RenderWindow* renderWindow, Shader* shader)
{
    mEntityManager = new EntityManager(mainWindow, renderWindow, shader);
    mCollisionManager = new CollisionManager();
    mMovementSystem = new MovementSystem(this);
    mMainWindow  = mainWindow;
    mRenderWindow = renderWindow;

    mShader[0] = &shader[0];
    mShader[1] = &shader[1];
    mShader[2] = &shader[2];
    mShader[3] = &shader[3];

    mMousePicker = new MousePicker(mRenderWindow, mRenderWindow->getCurrentCamera());
}

CoreEngine::~CoreEngine()
{
    delete mEntityManager;
    mEntityManager = nullptr;

    delete mCollisionManager;
    mCollisionManager = nullptr;
}

void CoreEngine::render()
{
    mMovementSystem->update();
    for(int i = 0; i < mEntityManager->getEntities().size(); i++)
    {
        mEntityManager->render(i);

    }
//    //    mEntityManager->render(0);
//std::cout << " DID IT WORK? " << checkCollision() << std::endl;

    mMousePicker->update();
    checkMousePicker();
//    std::cout << "Mouse Picker test " << checkMousePicker() << std::endl;
}
void CoreEngine::startGame()
{
    if (isRunning)
    {
        runGame();
    }
}

void CoreEngine::stopGame()
{
    if (!isRunning)
    {
        isRunning = false;
    }
}

void CoreEngine::runGame()
{
    if (isRunning == true)
    {
        while (mMainWindow->close())
        {
            stopGame();

        }
    }
}

bool CoreEngine::checkCollision()
{
    std::vector<Entity* >tempEntitis = mEntityManager->getEntities();
    for(int i = 0; i < tempEntitis.size() -1 ; i++)
    {
        if(tempEntitis.at(i)->getCollComp() != nullptr && tempEntitis.at(i+1)->getCollComp() != nullptr)
        {
            return mCollisionManager->checkCollision(tempEntitis.at(i)->getCollComp(), tempEntitis.at(i)->getTransComp(), tempEntitis.at(i+1)->getCollComp(), tempEntitis.at(i+1)->getTransComp());
        }
    }

    return false;
}
bool CoreEngine::checkMousePicker()
{
    std::vector<Entity* > mEntityObj = mEntityManager->getEntities();
    for(int i = 0; i < mEntityObj.size(); i++)
    {
        if(mEntityObj.at(i)->getCollComp() != nullptr && mEntityObj.at(i)->getTransComp() != nullptr )
        {
            // Checks for different distances and expands with 0.1
            for(float distanceRay = 0.0f; distanceRay < 20; distanceRay = distanceRay+0.1f)
            {
                if(mCollisionManager->pointToSphere(mEntityObj.at(i)->getCollComp(),mEntityObj.at(i)->getTransComp(), mMousePicker->getPointOnRay(distanceRay, mMousePicker->getCurrentRay())))
                {
                   pickedEntityID = mEntityObj.at(i)->entityID;
                    return true;
                }

            }

        }
    }
    pickedEntityID = -1;
    return false;
}

