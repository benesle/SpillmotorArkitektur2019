#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QString>
//#include <QTextBlock>

class QWidget;
class RenderWindow;
class QTreeWidget;
class QTreeWidgetItem;
class Entity;
class EntityWidget;

namespace Ui {
class EntityWidget;
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //    explicit MainWindow(QObject* parent, std::string);
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addItemsInTree();
    void addItemToTree(std::string, Entity*);
    void updateTree(std::vector<Entity*> childEntity);
    void updateEntityID(std::vector<Entity*> &childEntity);
    void addChildTree(QTreeWidgetItem* mItem, Entity* inChild);
    void setSelectedItemInTreeView(int EntID);
    void setEntityWidget(int entityID);
    void MousePickerSetActiveEntity(int entityID);

    std::vector<QString>objectNames;
    std::vector <QTreeWidgetItem*> itemList;

    bool isAddCube = false;
    bool isAddSphere = false;
    bool isAddPlane = false;
    bool isAddTriangle = false;



private slots:
    void on_pushButton_clicked();

    void on_pushButtonClose_clicked();

    void on_treeWidgetLOO_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionAdd_Triangle_triggered();

    void on_actionAdd_Plane_triggered();

    void on_actionAdd_Cube_triggered();

    void on_actionAdd_Sphere_triggered();

    void on_playButton_clicked();

    void on_pushButton_2_clicked();

private:
    void init();
    Ui::MainWindow *ui;

//This List
   std::vector<std::pair<QTreeWidgetItem*, Entity* >> uiEntityList;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
    EntityWidget* mEntityWid{nullptr};
};

#endif // MAINWINDOW_H
