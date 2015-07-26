#include "mycallback.h"

MyCallback::MyCallback(QObject *parent) :
    Sbs2Callback(parent)
{
    // MRA
    samplesToCollect = 8; // from 4 to 8
    pcaOn = 0;

    samples = 0;
    valuesIndex = 0;

    for (int buffer = 0; buffer < 100; ++buffer)
    {
	values.append(QVector<QList<int> >());
	for (int i=0; i<Sbs2Common::channelsNo(); ++i)
	{
	    values[buffer].append(QList<int>());
	    for (int k=0; k<this->samplesToCollect; ++k)
		values[buffer].last().append(0);
	}
    }

    for (int channel = 0; channel < Sbs2Common::channelsNo(); ++channel)
    {
	valuesSpectro.append(QVector<double>());
	for (int bin = 0; bin < Sbs2Common::samplingRate()/2; ++bin)
	{
	    valuesSpectro.last().append(0.0);
	}
    }

    QObject::connect(sbs2DataHandler,SIGNAL(spectrogramUpdated()),this,SLOT(spectrogramUpdatedSlot()));
    QObject::connect(sbs2DataHandler,SIGNAL(pcaUpdated()),this,SLOT(pcaUpdatedSlot()));

}

void MyCallback::getData(Sbs2Packet *packet)
{
    setPacket(packet);
    sbs2DataHandler->filter();
    sbs2DataHandler->pca_filter();
    sbs2DataHandler->spectrogramChannel();

    emit gyroSignal(thisPacket->gyroX, thisPacket->gyroY);

    if(!pcaOn)
    {
    for (int i=0; i<Sbs2Common::channelsNo(); ++i)
    {
	values[valuesIndex][i][samples] = thisPacket->filteredValues[Sbs2Common::getChannelNames()->at(i)];
    }

    ++samples;

    if (!(thisPacket->cq == -1))
	emit cqValue(thisPacket->cqName, thisPacket->cq);

    if (samples == samplesToCollect)
    {
	emit valueSignal(valuesIndex);
	valuesIndex = (valuesIndex+1)%values.size();
	samples = 0;
    }
    }

}

void MyCallback::pcaUpdatedSlot()
{
    if(!pcaOn)
        return;
    
    for (int i=0; i<Sbs2Common::channelsNo(); ++i)
    {
        for(int s = 0; s < samplesToCollect; s++)
        {
            values[valuesIndex][i][s] = (*sbs2DataHandler->getPcaValues())[s][i];
        }
    }

    emit valueSignal(valuesIndex);
    valuesIndex = (valuesIndex+1)%values.size();
}

void MyCallback::spectrogramUpdatedSlot()
{
    for (int channel = 0; channel < sbs2DataHandler->getPowerValues()->dim1(); ++channel)
    {
	for (int bin = 0; bin < sbs2DataHandler->getPowerValues()->dim2(); ++bin)
	{
	    valuesSpectro[channel][bin] = (*sbs2DataHandler->getPowerValues())[channel][bin];
	}
    }
    emit spectrogramUpdated();
}

// MRA
void MyCallback::turnPcaOnSlot()
{
    qDebug() << "MyCallBack: Turning pca on...";
    sbs2DataHandler->turnPcaOn(12000);
    pcaOn = 1;

    // MRA TODO: Add signals to data handler as well
}

// MRA
void MyCallback::turnPcaOffSlot()
{
    qDebug() << "MyCallBack: Turning pca off...";
    sbs2DataHandler->turnPcaOff();
    pcaOn = 0;

    // MRA TODO: Add signals to data handler as well
}
