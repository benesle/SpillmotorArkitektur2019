#include "entitywidget.h"
#include "mainwindow.h"
#include "ui_entitywid.h"
#include "entity.h"
#include "Components/transformcomponent.h"


EntityWidget::EntityWidget(Entity *ent, MainWindow *mainW, QWidget *parent) : mEntity(ent), mainwindow(mainW), QWidget(parent), ui(new Ui::EntityForm)
{
 ui->setupUi(this);
 setValue();

}

void EntityWidget::setLabel(QString name)
{
    std::string tempString = mEntity->entityName;
    ui->Entity->setText(QString::fromStdString(tempString));;

}

void EntityWidget::addChildren()
{
    ui->Children->clear();

    for(int i =0; i < mEntity->mChildren.size(); i++)
    {
       std::string tempString = (mEntity->mChildren.at(i)->entityName);
       ui->Children->addItem(QString::fromStdString(tempString));
    }
    if(mEntity->mChildren.empty())
    {
        ui->Children->addItem("No Children :'(");
    }
}

void EntityWidget::setValue()
{
    setLabel("Object");
    addChildren();
    ui->xValue->setValue(mEntity->getTransComp()->modelMatrix().getPosition().x);
    ui->yValue->setValue(mEntity->getTransComp()->modelMatrix().getPosition().y);
    ui->zValue->setValue(mEntity->getTransComp()->modelMatrix().getPosition().z);
}

void EntityWidget::update(Entity *mEntItem)
{
    mEntity = mEntItem;
    setValue();

}

void EntityWidget::on_xValue_valueChanged(double arg1)
{
    mEntity->getTransComp()->modelMatrix();
    auto &tempMatrix =  mEntity->getTransComp()->modelMatrix();
    tempMatrix.setPosition(static_cast<float>(arg1), tempMatrix.getPosition().y, tempMatrix.getPosition().z);

}

void EntityWidget::on_yValue_valueChanged(double arg1)
{
    mEntity->getTransComp()->modelMatrix();
    auto &tempMatrix =  mEntity->getTransComp()->modelMatrix();
    tempMatrix.setPosition(tempMatrix.getPosition().x, static_cast<float>(arg1), tempMatrix.getPosition().z);

}

void EntityWidget::on_zValue_valueChanged(double arg1)
{
    mEntity->getTransComp()->modelMatrix();
    auto &tempMatrix =  mEntity->getTransComp()->modelMatrix();
    tempMatrix.setPosition(tempMatrix.getPosition().x, tempMatrix.getPosition().y, static_cast<float>(arg1));
}
