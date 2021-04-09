#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H

#include <QWidget>

namespace Ui{class EntityForm;}

class MainWindow;
class Entity;
/**
 * The EntityWidget class
 */
class EntityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EntityWidget(Entity*,MainWindow*,QWidget *parent = nullptr);
    void setLabel(QString name);
    void addChildren();
    void setValue();
    void update(Entity*);

signals:

public slots:

private slots:
    void on_xValue_valueChanged(double arg1);

    void on_yValue_valueChanged(double arg1);

    void on_zValue_valueChanged(double arg1);

private:
    MainWindow* mainwindow;
    Ui::EntityForm* ui;
    Entity* mEntity;
};

#endif // ENTITYWIDGET_H
