#include "renderwindow.h"
#include "factory.h"
#include "soundcomponent.h"
#include "soundsource.h"

#include "meshcomponent.h"
#include "materialcomponent.h"
#include "transformcomponent.h"
#include "collisioncomponent.h"

#include "material.h"
#include "vertex.h"
#include <fstream>
#include <sstream>
//#include <fstream>
//#include <sstream>
#include "openal.h"
#include "entity.h"
#include "octahedronball.h"
#include "shader.h"
#include "soundmanager.h"
#include "wavefilehandler.h"


//Programmet skal også støtte å lese inn mesh-data fra filtypen obj.
///Valgfritt: Åpne en fildialog fra GUI der brukeren kan oppgi hvilken mesh som skal leses inn.

Factory::Factory()
{
    mMeshComponent.reserve(200);
    mBall = new OctahedronBall(2);
}

//Starten på en ressurshåndterer skal være laget
//så meshen til objektene nevnt over bare holdes én gang i RAM, og på GPU.
Factory::Factory(std::string filename)
{
    readFileTypeObj(filename);
    mMatrix.setToIdentity();

    if(filename.find(".obj") != std::string::npos)
    {
        readFileTypeObj(filename);
    }

    if (filename.find(".txt") != std::string::npos)

    {
        readFileTypeTxt(filename);
    }

    makeObject(filename);
}

Factory::~Factory()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void Factory::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Factory::draw()
{
    glUseProgram(mMaterial->mShader->getProgram());
    glBindVertexArray( mVAO );
    mMaterial->mShader->transmitUniformData(&mMatrix, mMaterial);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    //    glBindVertexArray(0);
}

void Factory::setShader(Shader *shader)
{
    mMaterial->mShader = shader;
}

void Factory::readFileTypeObj(std::string filename)
{
    //Open File
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;
    std::ifstream fileIn;
    fileIn.open (fileWithPath, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertecies;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    //    std::vector<Vertex> mVertices;    //made in VisualObject
    //    std::vector<GLushort> mIndices;   //made in VisualObject

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }

            //For some reason the winding order is backwards so fixing this by swapping the last two indices
            //Update: this was because the matrix library was wrong - now it is corrected so this is no longer needed.
            //            unsigned int back = mIndices.size() - 1;
            //            std::swap(mIndices.at(back), mIndices.at(back-1));
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();
    qDebug() << "Obj file read: " << QString::fromStdString(filename);
}

void Factory::readFileTypeTxt(std::string filename)
{
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);
        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
}

void Factory::readFileTypeTriangle(std::string filename)
{
    std::ifstream inn;
    std::string fileWithPath = gsl::assetFilePath + "Meshes/" + filename;

    //For collider:
    gsl::Vector3D RightUpBack{};
    gsl::Vector3D LeftDownFront{};

    inn.open(fileWithPath);

    if (inn.is_open()) {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            inn >> vertex;
            mVertices.push_back(vertex);

            //              //Set collider bounds:
            //              if(vertex.mXYZ.x < LeftDownFront.x)
            //                  LeftDownFront.x = vertex.mXYZ.x;
            //              if(vertex.mXYZ.y < LeftDownFront.y)
            //                  LeftDownFront.y = vertex.mXYZ.y;
            //              if(vertex.mXYZ.z > LeftDownFront.z)
            //                  LeftDownFront.z = vertex.mXYZ.z;
            //              if(vertex.mXYZ.x > RightUpBack.x)
            //                  RightUpBack.x = vertex.mXYZ.x;
            //              if(vertex.mXYZ.y > RightUpBack.y)
            //                  RightUpBack.y = vertex.mXYZ.y;
            //              if(vertex.mXYZ.z < RightUpBack.z)
            //                  RightUpBack.z = vertex.mXYZ.z;

        }
        inn.close();
        qDebug() << "TriangleSurface file read: " << QString::fromStdString(filename);
    }
    else
    {
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);
    }
}


void Factory::makeTriangle()
{
    mVertices.clear();
    mIndices.clear();
    //    initializeOpenGLFunctions();
    //        mMeshComponent.push_back(MeshComponent(0));

    mVertices.insert( mVertices.end(),
    {//Vertex data - normals not correct
     Vertex{gsl::Vector3D(-0.5f, -0.5f,  0.5f),    gsl::Vector3D(1.f, 0.f, 0.f),  gsl::Vector2D(0.f,  0.f)},   //Left low
     Vertex{gsl::Vector3D( 0.5f, -0.5f,  0.5f),    gsl::Vector3D(0.f, 1.f, 0.f),  gsl::Vector2D(1.f,  0.f)},   //Right low
     Vertex{gsl::Vector3D( 0.0f,  0.5f,  0.0f),    gsl::Vector3D(1.f, 1.f, 1.0f),  gsl::Vector2D(0.5f, 0.5f)},  //Top
     Vertex{gsl::Vector3D( 0.0f, -0.5f, -0.5f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f, 0.5f)}   //Back low
                      });

    mIndices.insert( mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
                     });

    //    openGLVertexBuffer();
    //    openGLIndexBuffer();

    //    mMeshComponent.back()->mVertices = mVertices.size();
    //    mMeshComponent.back().mIn = mIndices.size();
    //    mMeshComponent.back().mDrawType = GL_TRIANGLES;
    //    glBindVertexArray(0);

    //    mMeshMaxMinExtents.push_back(VectorPair{{.5f, .5f, -.5f},{-.5f, -.5f, .5f}});
}

void Factory::makeBall(int n)
{
    mVertices.clear();
    mIndices.clear();

    mVertices = mBall->vertices();
    //    initializeOpenGLFunctions();
    //       mMeshComponent.push_back(MeshComponent(0));
    //    GLint mRecursions = n;

    //    GLuint mNumberOfVertices = static_cast<GLuint>(3 * 8 * std::pow(4, mRecursions));     //unused

    //    makeBall(mRecursions);

    //       openGLVertexBuffer();
    //       openGLIndexBuffer();

    //       mMeshComponent.back().mVerticeCount = mVertices.size();
    //       mMeshComponent.back().mIndiceCount = mIndices.size();
    //       mMeshComponent.back().mDrawType = GL_TRIANGLES;
    //       glBindVertexArray(0);

    //       mMeshMaxMinExtents.push_back(VectorPair{{1.f, 1.f, -1.f},{-1.f, -1.f, 1.f}});
}

void Factory::makeTriangleSurface(std::string filename)
{
    mVertices.clear();
    mIndices.clear();
    initializeOpenGLFunctions();
    //       mMeshComponent.push_back(MeshComponent(0));

    readFileTypeTriangle(filename);

    //set up buffers
    openGLVertexBuffer();

    //       mMeshComponent.back().mVerticeCount = mVertices.size();
    //       mMeshComponent.back().mDrawType = GL_TRIANGLES;
    //       glBindVertexArray(0);
}


void Factory::openGLVertexBuffer()
{

}

Factory &Factory::instance()
{
    static Factory *mInstance = new Factory();
    return *mInstance;
}

MeshComponent* Factory::makeMeshComponent(std::string filename, int entityID)
{
    mVertices.clear();
    mIndices.clear();

    tempMesh = new MeshComponent;
    auto search = mMeshComponentMap.find(filename);

    if(search != mMeshComponentMap.end())
    {
        tempMesh->mVAO = mMeshComponent[search->second]->mVAO;
        tempMesh->mEAB = mMeshComponent[search->second]->mEAB;
        tempMesh->mIndices = mMeshComponent[search->second]->mIndices;
        tempMesh->mVertices = mMeshComponent[search->second]->mVertices;

        tempMesh->EntityID = entityID;

        return tempMesh;
    }
    else
    {
        //Not made

        mMeshComponentMap.insert(std::pair<std::string,int>(filename, entityID ));
        mMeshComponentMap.emplace(filename, entityID);

        //        if (filename == "xyz")
        //        {
        //            makeXYZ();

        //        }

        if (filename.find(".obj") != std::string::npos)
            makeObject(filename);

        if (filename.find(".txt") != std::string::npos)
            makeTriangleSurface(filename);

        if (filename == "tetrahedron")
            makeTriangle();

        if (filename == "ball0")
        {
            makeBall(0);
        }
        tempMesh->mVertices = mVertices;
        tempMesh->mIndices = mIndices;

        qDebug() << "Vertex size: " << mVertices.size();

        init();
        tempMesh->mVAO = mVAO;
        tempMesh->mEAB = mEAB;
        tempMesh->EntityID = entityID;
        //        tempMesh->mName = filename;
        std::string tempName;
        tempName = filename.substr(0, filename.find('.'));
        tempName = tempName + "_" + std::to_string(entityID);
        tempMesh->mName = tempName;
    }
    mMeshComponent.push_back(tempMesh);
    return tempMesh;

    //Check if already made
    //Search inside list of mesh maps
    //If already made, then set(vertices, mIndices, mVAO, mVBO, mEBO)
    //make new

    //    bool isMade{false};

    //    if()

    //    auto search = mMeshComponentMap.find(filename);
    //    if(search != mMeshComponentMap.end())
    //        return mMeshComponent[search->second];

    //    //not found - make new
    //    //Add to Map
    //    mMeshComponentMap[filename] = mMeshComponent.size();

    //    return mMeshComponent.back();
}

MaterialComponent* Factory::makeMaterialComponent(Shader* shader, int entityID)
{
    tempMaterial = new MaterialComponent;
    tempMaterial->setShader(shader);
    tempMaterial->entityID = entityID;

    return tempMaterial;
}

TransformComponent *Factory::makeTransformComponent(gsl::Vector3D vec, int entityID)
{
    tempTransform = new TransformComponent;
    tempTransform->translate(vec);
    tempTransform->entityID = entityID;

    return tempTransform;

}

//    readFileTypeObj(filename);
//    mMatrix.setToIdentity();

//    if(filename.find(".obj") != std::string::npos)
//    {
//        readFileTypeObj(filename);
//    }

//    if (filename.find(".txt") != std::string::npos)

//    {
//        readFileTypeTxt(filename);
//    }
//}

SoundComponent* Factory::makeSoundComponent(std::string filename, int entityID)
{
    //        SoundComponent::getInstance()->init();
    gsl::Vector3D position{0,0,0};
    tempSound = new SoundComponent;
    tempSound->mEntityID = entityID;
    tempSound->mName = filename;
    tempSound->setSource(createSource(
                             "Stereo", position,
                             gsl::soundFilePath + filename, true, 1.0f)); //false for ikke loop
    return tempSound;
}

CollisionComponent *Factory::makeCollisionComponent(std::string collisionType, int entityID)
{
    mVertices.clear();
    mIndices.clear();

    std::string filename;

    if(collisionType == "BoundingSphere")
    {
        filename = "ball0";
        tempCollision = new CollisionComponent;
        tempCollision->collShape = CollisionShape::BoundingSphere;
    }

    else
    {
        return nullptr;
    }


    //    auto search = mMeshComponentMap.find(filename);

    //    if(search != mMeshComponentMap.end())
    //    {

    //        tempCollision->mVAO = mMeshComponent[search->second]->mVAO;
    //        tempCollision->mEAB = mMeshComponent[search->second]->mEAB;
    //        tempCollision->mIndices = mMeshComponent[search->second]->mIndices;
    //        tempCollision->mVertices = mMeshComponent[search->second]->mVertices;

    //        tempCollision->EntityID = entityID;

    //        return tempCollision;
    //    }
    //    else
    {
        //Not made

        //        mMeshComponentMap.insert(std::pair<std::string,int>(filename, entityID ));
        //        mMeshComponentMap.emplace(filename, entityID);

        //        if (filename == "xyz")
        //        {
        //            makeXYZ();

        //        }

        if (filename.find(".obj") != std::string::npos)
            makeObject(filename);

        if (filename.find(".txt") != std::string::npos)
            makeTriangleSurface(filename);

        if (filename == "tetrahedron")
            makeTriangle();

        if (filename == "ball0")
        {
            makeBall(0);
        }
        tempCollision->mVertices = mVertices;
        tempCollision->mIndices = mIndices;

        qDebug() << "Vertex size: " << mVertices.size();

        init();
        tempCollision->mVAO = mVAO;
        tempCollision->mEAB = mEAB;
        tempCollision->EntityID = entityID;

        std::string tempName;
        tempName = filename.substr(0, filename.find('.'));
        tempName = tempName + "_" + std::to_string(entityID);
        tempCollision->mName = tempName;
    }
    mCollisionComponent.push_back(tempCollision);
    return tempCollision;
}

void Factory::playSound()
{
    //    getCameraInfo();
    //    //plays the sounds
    //    mSound->play();
    //    gsl::Vector3D position{0,0,0};
    //    mSound->setPosition(gsl::Vector3D(position));

}
void Factory::makeObject(std::string filename)

{
    //Make object for the first time
    //To recognize object, remember file name ex "monkey.obj "
    //Make a list with object names
    //When make object add name to list
    //Save which objects you have, so you know which one to not
    //Get information VAO, verteciesSize, indicesSize
    //readFile

    readFileTypeObj(filename);
    mMatrix.setToIdentity();

    if(filename.find(".obj") != std::string::npos)
    {
        readFileTypeObj(filename);
    }

    if (filename.find(".txt") != std::string::npos)

    {
        readFileTypeTxt(filename);
    }
}
void Factory::getCameraInfo()
{
    cameraSpeed = mRenderWindow->getCameraSpeed();
    cameraForward = mRenderWindow->getCameraForward();
    cameraPosition = mRenderWindow->getCameraPosition();
    cameraUp = mRenderWindow->getCameraUp();
}

void Factory::updateSoundListener()
{
    getCameraInfo();

    SoundManager::getInstance()->updateListener(cameraPosition, cameraSpeed, cameraForward, cameraUp);
}

SoundSource *Factory::createSource(std::string name, gsl::Vector3D pos, std::string filePath, bool loop, float gain)
{
    SoundSource* tempPtr = new SoundSource(name, loop, gain);
    tempPtr->setPosition(pos);
    if (filePath != "")
        tempPtr->loadWave(filePath);
    return tempPtr;
}
