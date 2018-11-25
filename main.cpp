#include <QApplication>

#include <mycallback.h>
#include <mainwindow.h>

#include <hardware/emotiv/sbs2emotivdatareader.h>
#include <hardware/filereader/sbs2filedatareader.h>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    QCommandLineOption dataFilePath("datafile","File to read instead of device", "filepath");
    parser.addOption(dataFilePath);
    parser.addHelpOption();
    parser.process(app);

    qDebug() << "catalogPath: "<<Sbs2Common::setDefaultCatalogPath();
    qDebug() << "rootAppPath: "<<Sbs2Common::setDefaultRootAppPath();

    MyCallback* myCallback = new MyCallback();
    Sbs2DataReader* sbs2DataReader = nullptr;
    if (!parser.isSet(dataFilePath))
    {
        sbs2DataReader = Sbs2EmotivDataReader::New(myCallback,0);
    }
    else
    {
        sbs2DataReader = new Sbs2FileDataReader(myCallback,parser.value(dataFilePath));
    }

    MainWindow* mw = new MainWindow(myCallback);
    mw->setAttribute(Qt::WA_QuitOnClose);

    QObject::connect(&app,SIGNAL(aboutToQuit()),mw->glwidget,SLOT(kill()));
    QObject::connect(&app,SIGNAL(aboutToQuit()),sbs2DataReader,SLOT(aboutToQuit()));
    QObject::connect(myCallback,SIGNAL(valueSignal(int)),mw->glwidget,SLOT(update(int)));
    QObject::connect(myCallback,SIGNAL(gyroSignal(int,int)),mw->glwidget,SLOT(gyroSlot(int,int)));
    QObject::connect(mw->glwidget, SIGNAL(turnFilterOn(int, int)),
                     myCallback, SLOT(turnFilterOn(int, int)));
    QObject::connect(mw->glwidget,SIGNAL(turnFilterOff()),myCallback,SLOT(turnFilterOff()));
    QObject::connect(mw->glwidget,SIGNAL(turnSpectrogramOn(int,int,int)),myCallback,SLOT(turnChannelSpectrogramOn(int,int,int)));
    QObject::connect(mw->glwidget,SIGNAL(turnSpectrogramOff()),myCallback,SLOT(turnChannelSpectrogramOff()));
    QObject::connect(myCallback,SIGNAL(spectrogramUpdated()),mw->glwidget,SLOT(updateSpectro()));

    return app.exec();
}
