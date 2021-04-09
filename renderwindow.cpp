#include "innpch.h"
#include "renderwindow.h"

#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <chrono>
#include <QWindow>

#include "factory.h"
#include "mainwindow.h"
#include "visualobject.h"
#include "texture.h"
#include "light.h"
#include "skybox.h"
#include "billboard.h"
#include "meshcomponent.h"
#include "materialcomponent.h"
#include "transformcomponent.h"
#include "colorshader.h"
#include "textureshader.h"
#include "phongshader.h"
#include "trianglesurface.h"
#include "octahedronball.h"
#include "xyz.h"
#include "objmesh.h"

#include "entitymanager.h"

#include "input.h"
#include "entity.h"
#include "camera.h"
#include "mousepicker.h"
#include "coreengine.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)
{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    for (int i = 0; i < 4; ++i) {
        if (mShaderProgram[i])
            delete mShaderProgram[i];
    }
}

/// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info:
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    mShaderProgram[0] = new ColorShader("plainshader");
    qDebug() << "Plain shader program id: " << mShaderProgram[0]->getProgram();
    mShaderProgram[1]= new TextureShader("textureshader");

    qDebug() << "Texture shader program id: " << mShaderProgram[1]->getProgram();
    mShaderProgram[2]= new PhongShader("phongshader");
    qDebug() << "Phong shader program id: " << mShaderProgram[2]->getProgram();

    //**********************  Texture stuff: **********************

    mTexture[0] = new Texture("white.bmp");
    mTexture[1] = new Texture("hund.bmp", 1);
    mTexture[2] = new Texture("skybox.bmp", 2);

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTexture[2]->id());




    //********************** Making the objects to be drawn **********************

    mCoreEng = new CoreEngine(mMainWindow, this, *mShaderProgram);
    mMainWindow->setEntityWidget(0);
    //Det skal være enkelt å legge til standard objekter som kube, kule, plan til prosjektet.
    //Kode som støtter dette skal være robust og gjennomtenkt.
    //Lag en måte å legge til disse objektene på fra GUI.
    mFactory = new Factory;
    //    mFactory->makeSoundComponent("sound",0);

    std::string filename;

    VisualObject * temp{nullptr};
    //    Entity * newtemp{nullptr};

    //    newtemp = new XYZ();

    // entities->attachChild(entities);
    //mMainWindow->updateTree(entities);

    temp = new XYZ();
    temp->init();
    temp->setShader(mShaderProgram[0]);
    mVisualObjects.push_back(temp);

    //    temp = new OctahedronBall(2);
    //    temp->init();
    //    temp->setShader(mShaderProgram[0]);
    //    temp->mMatrix.scale(0.5f, 0.5f, 0.5f);
    //    temp->mName = "Ball";
    //    mVisualObjects.push_back(temp);
    //    mPlayer = temp;

    temp = new SkyBox();
    temp->init();
    temp->setShader(mShaderProgram[1]);
    temp->mMaterial.setTextureUnit(2);
    temp->mMatrix.scale(15.f);
    temp->mName = "Cube";
    mVisualObjects.push_back(temp);

    temp = new BillBoard();
    temp->init();
    temp->setShader(mShaderProgram[1]);
    temp->mMatrix.translate(4.f, 0.f, -3.5f);
    temp->mName = "Billboard";
    temp->mRenderWindow = this;
    temp->mMaterial.setTextureUnit(1);
    temp->mMaterial.mObjectColor = gsl::Vector3D(0.7f, 0.6f, 0.1f);
    dynamic_cast<BillBoard*>(temp)->setConstantYUp(true);
    mVisualObjects.push_back(temp);

    mLight = new Light();
    temp = mLight;
    temp->init();
    temp->setShader(mShaderProgram[1]);
    temp->mMatrix.translate(2.5f, 3.f, 0.f);
    //    temp->mMatrix.rotateY(180.f);
    temp->mName = "light";
    temp->mRenderWindow = this;
    temp->mMaterial.setTextureUnit(0);
    temp->mMaterial.mObjectColor = gsl::Vector3D(0.1f, 0.1f, 0.8f);
    mVisualObjects.push_back(temp);

    static_cast<PhongShader*>(mShaderProgram[2])->setLight(mLight);

    //    //testing triangle surface class
    //    temp = new TriangleSurface("box2.txt");
    //    temp->init();
    //    temp->mMatrix.rotateY(180.f);
    //    temp->setShader(mShaderProgram[0]);
    //    mVisualObjects.push_back(temp);

    //one monkey
    temp = new ObjMesh("monkey.obj");
    temp->setShader(mShaderProgram[2]);
    temp->init();
    temp->mName = "Monkey";
    temp->mMatrix.scale(0.5f);
    temp->mMatrix.translate(3.f, 2.f, -2.f);
    mVisualObjects.push_back(temp);



    //--------------------------------------------------------------------------------------
    //Adding Object name in Widget

    //    for(auto obj: mVisualObjects)
    //    {
    //        mMainWindow->objectNames.push_back(QString::fromStdString(obj->mName));
    //    }
    for(auto tempItem: mCoreEng->getEntityManager()->getEntities())
    {
        mMainWindow->addItemToTree(tempItem->entityName, tempItem);
        std::cout << tempItem->entityName;
    }
    mMainWindow->addItemsInTree();



    //********************** Set up camera **********************
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(6.f, 2.f, 50.f));
    //    mCurrentCamera->yaw(45.f);
    mCurrentCamera->pitch(10.f);

    //new system - shader sends uniforms so needs to get the view and projection matrixes from camera
    mShaderProgram[0]->setCurrentCamera(mCurrentCamera);
    mShaderProgram[1]->setCurrentCamera(mCurrentCamera);
    mShaderProgram[2]->setCurrentCamera(mCurrentCamera);

    //MousePicker, move selected OBJ
    //   mMousePicker = new MousePicker(this, mCurrentCamera);

}

///Called each frame - doing the rendering
void RenderWindow::render()
{
    ///    calculate the time since last render-call
    ///    this should be the same as xxx in the mRenderTimer->start(xxx) set in RenderWindow::exposeEvent(...)
    ///        auto now = std::chrono::high_resolution_clock::now();
    ///        std::chrono::duration<float> duration = now - mLastTime;
    ///        std::cout << "Chrono deltaTime " << duration.count()*1000 << " ms" << std::endl;
    ///        mLastTime = now;

    //input

    //Programmet skal spille 3D-lyd. Dvs. minst ett objekt
    //skal lage lyd, og når en styrer kamera nære dette
    //objektet så hører en panorering og styrkeforskjeller.
    handleInput();
    //    if(soundOn == false)
    //    {
    //                mFactory->playSound();
    //        soundOn = true;
    //    }

    //        mFactory->updateSoundListener();

    mCurrentCamera->update();

    /* mMousePicker->update();*/ //Remember to UpdateMouse picker after the camera has moved
    //    std::cout << mMousePicker->getCurrentRay();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCoreEng->render();
    VisualObject *temp{nullptr};

    if(mMainWindow->isAddCube == true)
    {

        temp = new TriangleSurface("box2.txt");
        temp->init();
        temp->mMatrix.rotateY(180.f);
        temp->setShader(mShaderProgram[0]);
        mVisualObjects.push_back(temp);

        mMainWindow->isAddCube = false;

    }

    if(mMainWindow->isAddPlane == true)
    {

        temp = new TriangleSurface();
        temp->init();
        //    temp->mMatrix.rotateY(180.f);
        temp->setShader(mShaderProgram[0]);
        temp->mMatrix.translateX(2);
        temp->mMatrix.scale(20);
        mVisualObjects.push_back(temp);

        mMainWindow->isAddCube = false;

    }

    if(mMainWindow->isAddSphere == true)
    {
        temp = new OctahedronBall(2);
        temp->init();
        temp->setShader(mShaderProgram[0]);
        temp->mMatrix.scale(0.5f, 0.5f, 0.5f);
        temp->mMatrix.translateY(5);
        temp->mName = "Ball";
        mVisualObjects.push_back(temp);
    }

    //    for (auto visObject: mVisualObjects)
    //    {
    //        visObject->draw();
    //        //        checkForGLerrors();
    //    }

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    //    checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    //    auto start = std::chrono::high_resolution_clock::now();
    mContext->swapBuffers(this);
    //    auto end = std::chrono::high_resolution_clock::now();
    //    std::chrono::duration<float> duration = end - start;
    //    std::cout << "Chrono deltaTime " << duration.count()*1000 << " ms" << std::endl;

    //    calculateFramerate();

}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

Camera *RenderWindow::getCurrentCamera() const
{
    return mCurrentCamera;
}

float RenderWindow::getMouseX()
{
    return mMouseX;

}

float RenderWindow::getMouseY()
{
    return mMouseY;
}

CoreEngine *RenderWindow::getCoreEng() const
{
    return mCoreEng;
}

float RenderWindow::cameraSpeed() const
{
    return mCameraSpeed;
}

//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mRenderTimer->start(1);
        mTimeStart.start();
    }
    mAspectratio = static_cast<float>(width()) / height();
    //    qDebug() << mAspectratio;
    mCurrentCamera->mProjectionMatrix.perspective(45.f, mAspectratio, 1.f, 100.f);
    //    qDebug() << mCamera.mProjectionMatrix;
}

//Simple way to turn on/off wireframe mode
//Not totally accurate, but draws the objects with
//lines instead of filled polygons
void RenderWindow::toggleWireframe()
{
    mWireframe = !mWireframe;
    if (mWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //turn on wireframe mode
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //turn off wireframe mode
        glEnable(GL_CULL_FACE);
    }
}

//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000., 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

/// Uses QOpenGLDebugLogger if this is present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }

        if(mOpenGLDebugLogger)
            mOpenGLDebugLogger->disableMessages(QOpenGLDebugMessage::APISource, QOpenGLDebugMessage::OtherType, QOpenGLDebugMessage::NotificationSeverity);
    }
}

void RenderWindow::setCameraSpeed(float value)
{
    mCameraSpeed += value;

    //Keep within min and max values
    if(mCameraSpeed < 0.01f)
        mCameraSpeed = 0.01f;
    if (mCameraSpeed > 0.3f)
        mCameraSpeed = 0.3f;
}

void RenderWindow::handleInput()
{
    //Camera
    mCurrentCamera->setSpeed(0.f);  //cancel last frame movement
    if(mInput.RMB)
    {
        if(mInput.W)
            mCurrentCamera->setSpeed(-mCameraSpeed);
        if(mInput.S)
            mCurrentCamera->setSpeed(mCameraSpeed);
        if(mInput.D)
            mCurrentCamera->moveRight(mCameraSpeed);
        if(mInput.A)
            mCurrentCamera->moveRight(-mCameraSpeed);
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-mCameraSpeed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(mCameraSpeed);
    }
    else
    {
        if(mInput.W)
            mLight->mMatrix.translateZ(-mCameraSpeed);
        if(mInput.S)
            mLight->mMatrix.translateZ(mCameraSpeed);
        if(mInput.D)
            mLight->mMatrix.translateX(mCameraSpeed);
        if(mInput.A)
            mLight->mMatrix.translateX(-mCameraSpeed);
        if(mInput.Q)
            mLight->mMatrix.translateY(mCameraSpeed);
        if(mInput.E)
            mLight->mMatrix.translateY(-mCameraSpeed);
    }

    //If picked entity = true
    if(mInput.LMB)
    {
        if(mCoreEng->pickedEntityID != -1)
        {
            mMainWindow->MousePickerSetActiveEntity(mCoreEng->pickedEntityID);
            oldPickedID = mCoreEng->pickedEntityID; //Denne må gjøres tilgjengelig i mainWindow slik at du kan sette den gamle til false.
            mCoreEng->pickedEntityID = -1;
        }
    }


    if(mInput.RIGHT)
    {
        if(mCoreEng->getEntityManager()->getEntities().at(0))
        {
            mCoreEng->getEntityManager()->getEntities().at(0)->getTransComp()->translate(gsl::Vector3D{0.01,0,0});
        }
    }

    {
        if(mInput.LEFT)
        {
            if(mCoreEng->getEntityManager()->getEntities().at(0))
            {
                mCoreEng->getEntityManager()->getEntities().at(0)->getTransComp()->translate(gsl::Vector3D{-0.01,0,0});
            }
        }
    }
    if(mInput.SPACE)
    {
        mCoreEng->getEntityManager()->getEntities().at(1)->getTransComp()->setPosition(mCoreEng->getEntityManager()->getEntities().at(0)->getTransComp()->getPosition());
        mCoreEng->getEntityManager()->getEntities().at(1)->getTransComp()->translate(gsl::Vector3D{0,0,-1});
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }

    //    You get the keyboard input like this
    if(event->key() == Qt::Key_W)
    {
        mInput.W = true;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = true;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = true;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = true;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = true;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = true;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = true;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = true;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = true;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = true;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Space)
    {
      mInput.SPACE = true;
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        mInput.W = false;
    }
    if(event->key() == Qt::Key_S)
    {
        mInput.S = false;
    }
    if(event->key() == Qt::Key_D)
    {
        mInput.D = false;
    }
    if(event->key() == Qt::Key_A)
    {
        mInput.A = false;
    }
    if(event->key() == Qt::Key_Q)
    {
        mInput.Q = false;
    }
    if(event->key() == Qt::Key_E)
    {
        mInput.E = false;
    }
    if(event->key() == Qt::Key_Z)
    {
    }
    if(event->key() == Qt::Key_X)
    {
    }
    if(event->key() == Qt::Key_Up)
    {
        mInput.UP = false;
    }
    if(event->key() == Qt::Key_Down)
    {
        mInput.DOWN = false;
    }
    if(event->key() == Qt::Key_Left)
    {
        mInput.LEFT = false;
    }
    if(event->key() == Qt::Key_Right)
    {
        mInput.RIGHT = false;
    }
    if(event->key() == Qt::Key_U)
    {
    }
    if(event->key() == Qt::Key_O)
    {
    }
    if(event->key() == Qt::Key_Space)
    {
      mInput.SPACE = false;
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = true;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = true;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        mInput.RMB = false;
    if (event->button() == Qt::LeftButton)
        mInput.LMB = false;
    if (event->button() == Qt::MiddleButton)
        mInput.MMB = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    //if RMB, change the speed of the camera
    if (mInput.RMB)
    {
        if (numDegrees.y() < 1)
            setCameraSpeed(0.001f);
        if (numDegrees.y() > 1)
            setCameraSpeed(-0.001f);
    }
    event->accept();
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mInput.RMB)
    {
        //Using mMouseXYlast as deltaXY so we don't need extra variables
        mMouseXlast = event->pos().x() - mMouseXlast;
        mMouseYlast = event->pos().y() - mMouseYlast;

        if (mMouseXlast != 0)
            mCurrentCamera->yaw(mCameraRotateSpeed * mMouseXlast);
        if (mMouseYlast != 0)
            mCurrentCamera->pitch(mCameraRotateSpeed * mMouseYlast);
    }
    mMouseXlast = event->pos().x();
    mMouseYlast = event->pos().y();

    mMouseX = event->pos().x();
    mMouseY = event->pos().y();

}
