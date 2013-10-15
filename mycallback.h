#ifndef MYCALLBACK_H
#define MYCALLBACK_H

#include <sbs2callback.h>
#include <sbs2datahandler.h>

class MyCallback : public Sbs2Callback
{
    Q_OBJECT
public:
    explicit MyCallback(QObject *parent = 0);
    void getData(Sbs2Packet *packet);
    QVector<QVector<QList<int> > > values;
    QVector<QVector<double> > valuesSpectro;

private:
    int samplesToCollect;
    int samples;
    int valuesIndex;

signals:
    void valueSignal(int index);
    void gyroSignal(int gyroX, int gyroY);
    void updateSpectro();
    
public slots:
    void spectrogramUpdatedSlot();

};

#endif // MYCALLBACK_H
