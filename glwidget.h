#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <sbs2common.h>
#include <mycallback.h>
#include <scalpmap.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(MyCallback* myCallback_, QWidget *parent = 0);


private:
    QBasicTimer *timer;

    QVector<QPainterPath*> channels;
    QVector<QPainterPath*> channelsSpectro;


    QVector<QLine*> gridLines;
    QVector<QLine*> gridLinesSpectro;

    int w;
    int h;
    double aspectRatio;
    int values;
    int valuesSpectro;
    int valueToValueDistance;
    int valueToValueSpectroDistance;
    int currentPosition;
    int pathGenerated;
    int pathSpectroGenerated;

    double gyroX;
    double gyroY;
    double gyroPosX;
    double gyroPosY;
    double gyroGravity;

    QPainter* painter;
    QVector<QList<int> > rawValues;
    QVector<QList<double> > rawValuesSpectro;

    int startY;
    double scale;
    double lastD;
    double startX;
    double currentX;

    MyCallback* myCallback;

    int paused;
    int timeSeries;
    int filterSetting;
    int scalpmapVisible;

    QRect toggleFilterRect;
    QRect toggleTimeSeriesRect;
    QRect togglePlayRect;
    QRect quitRect;
    QRect toggleHardwareRect;
    QRect toggleScalpmapRect;

    QString filterText;
    QString timeSeriesText;
    QString playText;
    QString scalpmapText;

    ScalpMap* scalpmap;

private:
    void paintGL ();
    void generatePaths();
    void generateGrid();
    void generatePathsSpectro();
    void generateGridSpectro();
    void generateUI();
    void initializeGL();
    void resizeGL(int w, int h);
    void timerEvent(QTimerEvent *);
    void toggleFilter();
    void toggleTimeSeries();
    void togglePlay();
    void toggleHardware();
    void toggleScalpmap();

    //Interaction
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);




signals:
    void turnFilterOn(int fbandLow, int fbandHigh, int order);
    void turnSpectrogramOn(int samples, int legnth, int delta);
    void turnFilterOff();
    void turnSpectrogramOff();

public slots:
     //void update(QVector<QList<int> > vs);
    void update(int index);
    void updateSpectro();
    void gyroSlot(int gyroX_, int gyroY_);
    void kill();


};

#endif // GLWIDGET_H
