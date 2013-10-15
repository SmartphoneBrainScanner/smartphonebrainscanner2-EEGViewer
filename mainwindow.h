#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>
#include <mycallback.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(MyCallback* myCallback, QWidget *parent = 0);
    GLWidget* glwidget;

private:

signals:

public slots:

};

#endif // MAINWINDOW_H
