#include "mycallback.h"

MyCallback::MyCallback(QObject *parent) :
    Sbs2Callback(parent)
{
    samplesToCollect = 4;
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

}

void MyCallback::getData(Sbs2Packet *packet)
{
    setPacket(packet);
    sbs2DataHandler->filter();
    sbs2DataHandler->spectrogramChannel();

    emit gyroSignal(thisPacket->gyroX, thisPacket->gyroY);

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
