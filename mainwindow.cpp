#include "innpch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderwindow.h"
#include "entitywidget.h"
#include "ui_entitywid.h"

#include <QMainWindow>
#include <QSurfaceFormat>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>

#include <QString>

#include "soundcomponent.h"

#include "entity.h"

#include "coreengine.h"
#include "entitymanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    //    QTreeWidgetItem * newitem = new QTreeWidgetItem;
    //    newitem->setText(0,"String");
    //ui->treeWidgetLOO->insertTopLevelItem(0,newitem);
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}
//GUI skal vise objektene på scenen i en liste.
void MainWindow::addItemsInTree()
{
    for(auto obj: objectNames)
    {
        QTreeWidgetItem * newitem = new QTreeWidgetItem;
        newitem->setText(0,obj);
        ui->treeWidgetLOO->insertTopLevelItem(0,newitem);
    }

}

void MainWindow::addItemToTree(std::string objName, Entity* objEntity)
{
    QTreeWidgetItem* addItem = new QTreeWidgetItem;
    addItem->setText(0, QString::fromStdString(objName));
    ui->treeWidgetLOO->insertTopLevelItem(0,addItem);
    uiEntityList.push_back({addItem, objEntity});
}
//void MainWindow::addChildTree(QTreeWidgetItem *mItem, Entity *childIn)
//{
//    //    for(auto item: itemList)
//    //    {
//    QTreeWidgetItem *mItems = new QTreeWidgetItem();
//    mItems->setText(0, QString::fromStdString(childIn->getEntityName()));
//    mItems->setText(1, QString::number(childIn->getEntityID()));

//    mItem->addChild(mItems);
//    itemList.push_back(mItems);
//    ui->treeWidgetLOO->insertTopLevelItem(0,mItems);

//    if(childIn->hasChild())
//    {
//        addChildTree(mItems, childIn->getChild());
//    }

//}

//Programmet skal ha mulighet til at ett objekt er child av et annet.
//Dette skal vises i programmet.
//Interaktivt, eller ved at et sammensatt objekt beveger seg av seg selv.
//void MainWindow::updateTree(std::vector<Entity *> childEntity)
//{
//    itemList.clear();
//    ui->treeWidgetLOO->clear();
//    updateEntityID(mRenderWindow->entities);

//    for (unsigned int i = 0; i < childEntity.size(); ++i)
//    {
//        QTreeWidgetItem* mItems = nullptr;

//        if (!childEntity[i]->hasParent())
//        {
//            mItems = new QTreeWidgetItem();
//            mItems->setText(0, QString::fromStdString(childEntity[i]->getEntityName()));
//            mItems->setText(1, QString::number(childEntity[i]->getEntityID()));

//            ui->treeWidgetLOO->addTopLevelItem(mItems);
//            itemList.push_back(mItems);
//        }

//        if (childEntity[i]->hasChild())
//        {
//            if (mItems != nullptr) //TODO, find more secure and robust way to fix this
//            {
//                addChildTree(mItems, childEntity[i]->mChild);
//                itemList.push_back(mItems);
//            }
//        }
//    }
//}

void MainWindow::updateEntityID(std::vector<Entity *> &childEntity)
{
    for (unsigned int i = 0; i < childEntity.size(); ++i)
    {
        childEntity[i]->setEntityID(i);
    }
}

//void MainWindow::setSelectedItemInTreeView(int EntID)
//{
//    QTreeWidgetItem* mItem;
//    for (auto item : itemList)
//    {
//        if ((item->text(1).toInt() == EntID))
//        {
//            qDebug("NOE");
//            mItem = item;
//            ui->treeWidgetLOO->setCurrentItem(mItem);
//            mRenderWindow->selectedEntity = mRenderWindow->entities[EntID];
//        }
//    }
//}

void MainWindow::setEntityWidget(int entityID)
{
    Entity* temp = mRenderWindow->getCoreEng()->getEntityManager()->findEntityByID(entityID);

    delete mEntityWid;
    mEntityWid = nullptr;

    mEntityWid = new EntityWidget(temp,this);
    //    ui->RightContent->addWidget(mEntityWid);
    ui->RightWid->layout()->addWidget(mEntityWid);
    mEntityWid->show();
}

void MainWindow::MousePickerSetActiveEntity(int entityID)
{

    if(mRenderWindow->oldPickedID != -1)
    {
        ui->treeWidgetLOO->topLevelItem(mRenderWindow->oldPickedID)->setSelected(false);
    }
    setEntityWidget(entityID);
    ui->treeWidgetLOO->topLevelItem(entityID)->setSelected(true);

}




//    /*int EntityId = 0;
//virtual void Add(Entity *a);
//virtual void Remove();
//virtual void Delete(Entity *a);
//std::vector<Entity* > children;
//*/

//    //Component -Declare interface for objects, accessing and managing childs
//    //Leaf -Defines behaviour for primitive components
//    //Composite -Store Child components and implements child related operations
//    //Client -Manipulates the objects through the component interface
//    //void Entity::Add(Entity *a)
//    //{
//    //    children.push_back(a);
//    //}

//    //void Entity::Delete(Entity *a)
//    //{
//    //   for( int i = 0; i < children.size(); i++)
//    //   {
//    //       if (children.at(i)->EntityId == a->EntityId)
//    //       {
//    //           children.erase(children.begin()+i);

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this...)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Set the number of samples used for multisampling
    format.setSamples(8);

    //Turn off VSync. If this is set to 1, VSync is on - default behaviour
    format.setSwapInterval(0);

    //Just prints out what OpenGL format we will get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context())
    {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //Set size of program in % of available screen
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);



}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_clicked()
{
    mRenderWindow->toggleWireframe();
}

void MainWindow::on_pushButtonClose_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure you want to quit the application?","This will close the application.",
                                                              QMessageBox::Yes | QMessageBox::No);
    if( reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
    else
    {
        qDebug() << "Clicked No";
    }
}


void MainWindow::on_actionAdd_Triangle_triggered()
{
    //Implement later
}

void MainWindow::on_actionAdd_Plane_triggered()
{
    //TODO
    //Rydd opp i floating pointers
    //Legg til i header
    isAddPlane = true;

    QTreeWidgetItem * newitem = new QTreeWidgetItem;
    newitem->setText(0,"Plane");
    ui->treeWidgetLOO->insertTopLevelItem(0,newitem);
}

void MainWindow::on_actionAdd_Cube_triggered()
{
    QTreeWidgetItem * newitem = new QTreeWidgetItem;
    isAddCube = true;
    newitem->setText(0,"Cube");
    ui->treeWidgetLOO->insertTopLevelItem(0,newitem);
}

void MainWindow::on_actionAdd_Sphere_triggered()
{
    QTreeWidgetItem * newitem = new QTreeWidgetItem;
    isAddSphere = true;
    newitem->setText(0,"Sphere");
    ui->treeWidgetLOO->insertTopLevelItem(0,newitem);
}

void MainWindow::on_treeWidgetLOO_itemClicked(QTreeWidgetItem *item, int column)

{
    qDebug("FUNKER DET?") ;

    Entity* thisEntity = nullptr;
    for(auto entityList: uiEntityList)
    {
        if(entityList.first == item)
        {
            thisEntity = entityList.second;
            break;
        }
    }
    if( thisEntity != nullptr)
    {
        mEntityWid->update(thisEntity);
    }

}


void MainWindow::on_playButton_clicked()
{
//    SoundComponent* soundComp;
    mRenderWindow->getCoreEng()->isRunning = true;

}

void MainWindow::on_pushButton_2_clicked()
{
    mRenderWindow->getCoreEng()->isRunning = false;
    mRenderWindow->getCoreEng()->getEntityManager()->reset();
}
