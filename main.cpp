#include <QApplication>
#include "qmlapplicationviewer.h"
#include <QtDeclarative>

#include <mycallback.h>
#include <mainwindow.h>

#include <hardware/emotiv/sbs2emotivdatareader.h>
#include <hardware/fake/sbs2fakedatareader.h>

// MRA
//#define USE_FAKE_DATA 1

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    // MRA
    Sbs2Common::setRootAppPath("/home/michael/Work/Git/SBS2_Qt/data");

    qDebug() << "catalogPath: " << Sbs2Common::setDefaultCatalogPath();
    qDebug() << "rootAppPath: " << Sbs2Common::getRootAppPath();


    MyCallback* myCallback = new MyCallback();

    // MRA
#ifdef USE_FAKE_DATA
    Sbs2FakeDataReader* sbs2DataReader = Sbs2FakeDataReader::New(myCallback, 0);
    sbs2DataReader->setFilename("data.txt");
    sbs2DataReader->start();
#else
    Sbs2EmotivDataReader* sbs2DataReader = Sbs2EmotivDataReader::New(myCallback,0);
#endif

    MainWindow* mw = new MainWindow(myCallback);
    mw->setAttribute(Qt::WA_QuitOnClose);

    Sbs2Common::setHardware("emotiv");

    QObject::connect(app.data(),SIGNAL(aboutToQuit()),mw->glwidget,SLOT(kill()));
    QObject::connect(app.data(), SIGNAL(aboutToQuit()), sbs2DataReader, SLOT(aboutToQuit()));
    QObject::connect(myCallback,SIGNAL(valueSignal(int)),mw->glwidget,SLOT(update(int)));
    QObject::connect(myCallback,SIGNAL(gyroSignal(int,int)),mw->glwidget,SLOT(gyroSlot(int,int)));
    QObject::connect(mw->glwidget,SIGNAL(turnFilterOn(int,int,int)),myCallback,SLOT(turnFilterOn(int,int,int)));
    QObject::connect(mw->glwidget,SIGNAL(turnFilterOff()),myCallback,SLOT(turnFilterOff()));
    QObject::connect(mw->glwidget,SIGNAL(turnSpectrogramOn(int,int,int)),myCallback,SLOT(turnChannelSpectrogramOn(int,int,int)));
    QObject::connect(mw->glwidget,SIGNAL(turnSpectrogramOff()),myCallback,SLOT(turnChannelSpectrogramOff()));
    QObject::connect(myCallback,SIGNAL(spectrogramUpdated()),mw->glwidget,SLOT(updateSpectro()));

    // MRA
    QObject::connect(mw->glwidget, SIGNAL(turnPcaOn()), myCallback, SLOT(turnPcaOnSlot()));
    QObject::connect(mw->glwidget, SIGNAL(turnPcaOff()), myCallback, SLOT(turnPcaOffSlot()));

    int ret = app->exec();

#ifdef USE_FAKE_DATA
    sbs2DataReader->stop();
#endif

    return ret;
}
