#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "innpch.h"

class Entity;
class MainWindow;
class RenderWindow;
class Factory;
class MeshComponent;
class CollisionComponent;
class Shader;

/**
 *In this class the entities are made with their components.
 */
class EntityManager
{
public:
    //I denne klassen lager man alle entities, med komponenter.
    //Man bør vite om Renderwindow og mainwindow
    //Dette bør man derfor sende inn i konstruktoren.

    EntityManager(MainWindow*,RenderWindow*, Shader*);
    ~EntityManager();
    //Funksjoner:
    //Her lager du entities, ved å kjøre makeComponent/ makeObject? fra factory.
    //FileName

    //Make Objects with EntityManager, which works with UI
    void makeScene();

    void makeEnemy(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume);
    void spawnEnemis();
    void destroyEntity(int ID);
    void reset();

    void makeEntity(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume);
    void makePlayer(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume);
    void makeProjectile(std::string meshFilename, gsl::Vector3D position, int shaderIndex, std::string collisionVolume);
    void move(unsigned int ID);
    Shader* shaderProgram[4]{nullptr};
    void render(unsigned int ID);

    Entity *findEntityByID(unsigned ID);

    //rotation();,

    //MeshComponent
    //MaterialComponent
    //


    //collision(); //Component med actions ved collision
    //BoundryBox();//CollisionCompononent - lager box
    //Physics();
    //Render(); // Etter hvert RenderSystem som kjører det.


    std::vector<Entity *> getEntities() const;

private:
    //Lagre alle entities som blir laget
    std::vector<Entity *> mEntities;

    RenderWindow* mRenderWindow;
    MainWindow* mMainWindow;
    Factory* mFactory;

    int entityCount = 0;

    //REMEMBER TO DELETE "NEW" POINTER LATER
    //OR
    //MEMORYLEAK DOOM IS OPON US


    //Entity:
    //- Den vet om alle komponentene du har.
};

#endif // ENTITYMANAGER_H
