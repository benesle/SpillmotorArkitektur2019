#ifndef FACTORY_H
#define FACTORY_H
#include <QOpenGLFunctions_4_1_Core>
#include "innpch.h"
#include "openal.h"

namespace gsl

{}
class RenderWindow;
class Scene;
class Shader;
class Entity;
class Material;
class OctahedronBall;
class SoundComponent;
class MeshComponent;
class MaterialComponent;
class CollisionComponent;
class TransformComponent;
class QAbstractOpenGLFunctions;
class SoundSource;

/**
 * The Factory class
 * This is used to make just one instance of an object
 * Reads file types and sets the shader
 */
class Factory : public QOpenGLFunctions_4_1_Core
{

public:

    Factory();
    Factory(std::string filename);
    ~Factory();

    void init() ;
    void draw();

    void setShader(Shader *shader);
    void readFileTypeObj(std::string filename);
    void readFileTypeTxt(std::string filename);
    void readFileTypeTriangle(std::string filename);

    bool checkALError(std::string filename);
    std::map<std::string, int> mMeshComponentMap;
    std::map<std::string, int> mMaterialComponentMap;
    std::map<std::string, int> mSoundComponentMap;

    void makeObject(std::string fileName);
    void makeXYZ();
    void makeTriangle();
    void makeBall(int n = 0);
    void makeTriangleSurface(std::string filename);
    void makeCube();
    /**
     * makePlayer
     */
    void makePlayer();
    /**
     * makeEnemy
     */
    void makeEnemy();


    void openGLVertexBuffer();
    void openGLIndexBuffer();

    void getCameraInfo();

    gsl::Vector3D cameraPosition{0,0,0};
    gsl::Vector3D cameraForward{0,0,-1};
    gsl::Vector3D cameraUp{0,1,0};
    float cameraSpeed{0};


    gsl::Matrix4x4 mMatrix;
    std::string mName;
    std::vector<MeshComponent *> mMeshComponent;
    std::vector<MaterialComponent *> mMaterialComponent;
    std::vector<SoundComponent *> mSound;
    std::vector<CollisionComponent *> mCollisionComponent;

    RenderWindow *mRenderWindow; //Just to be able to call checkForGLerrors()
    Material *mMaterial;
    Scene *mScene{nullptr};
    OctahedronBall *mBall{nullptr};

    static Factory& instance();

    MeshComponent *makeMeshComponent(std::string filename, int entityID);
    MaterialComponent *makeMaterialComponent(Shader *, int entityID);
    TransformComponent* makeTransformComponent(gsl::Vector3D, int entityID);
    SoundComponent *makeSoundComponent(std::string filename, int entityID);
    CollisionComponent* makeCollisionComponent(std::string collisionType, int entityID);

//    SoundSource* mSound{};
    void playSound();
    void updateSoundListener();
    SoundSource* createSource(std::string name,  gsl::Vector3D pos, std::string filePath = "", bool loop = false, float gain = 1.0);
protected:

    std::vector<Vertex> mVertices;   //This is usually not needed after object is made
    std::vector<GLuint> mIndices;    //This is usually not needed after object is made

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

    MeshComponent* tempMesh{nullptr};
    MaterialComponent* tempMaterial{nullptr};
    TransformComponent* tempTransform{nullptr}; //Husk delete -Destructor
    CollisionComponent* tempCollision{nullptr};
    SoundComponent* tempSound{nullptr};

};


#endif // FACTORY_H
