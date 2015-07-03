#include "mainwindow.h"

MainWindow::MainWindow(MyCallback *myCallback, QWidget *parent) :
    QMainWindow(parent)
{
    glwidget = new GLWidget(myCallback);

    this->resize(1280,700);
    glwidget->resize(this->size());

    glwidget->show();

    close();
}
