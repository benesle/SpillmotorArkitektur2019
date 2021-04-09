#ifndef COREENGINE_H
#define COREENGINE_H

#include <QDebug>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QString>

class QWidget;
class RenderWindow;
class MainWindow;
class Shader;
class EntityManager;
class MousePicker;
class CollisionManager;
class MovementSystem;
/**
 * The CoreEngine class
 *
 */
class CoreEngine : public QObject
{
    Q_OBJECT

private:
    EntityManager* mEntityManager;
    CollisionManager* mCollisionManager;
    MousePicker* mMousePicker;
    MovementSystem* mMovementSystem;

public:

    MainWindow *mMainWindow{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    Shader *mShader[4]{nullptr};

    CoreEngine(MainWindow*mainWindow, RenderWindow*renderWindow, Shader*shader);
    virtual ~CoreEngine();

    bool isRunning = false;
    bool isClosed =false;
    void render();
    void startGame();
    void stopGame();
    void runGame();
    bool checkCollision();
    bool checkMousePicker();
    int pickedEntityID = -1;

    EntityManager *getEntityManager() const;
    CollisionManager *getCollisionManager();

};
#endif // COREENGINE_H
