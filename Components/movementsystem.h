#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H
#include "innpch.h"

class CoreEngine;
class CollisionManager;
class TransformComponent;
class Entity;
/**
 * The MovementSystem class
 */
class MovementSystem
{
public:
    /**
     * MovementSystem
     * @param mCoreEngine
     */
    MovementSystem(CoreEngine* mCoreEngine);
    ~MovementSystem();

    /**
     * enemyMove
     * A function used to move the enemy
     */
    void enemyMove();
    /**
     * projectileMovement
     */
    void projectileMovement();

    void update();
    void move();
    CoreEngine* coreEngine;
};

#endif // MOVEMENTSYSTEM_H
