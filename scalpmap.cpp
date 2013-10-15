#include "scalpmap.h"

ScalpMap::ScalpMap(int scalling_, QWidget *parent) :
    QWidget(parent), scalling(scalling_)
{
    scalpmapPixmap = new QPixmap(":/scalpmap.png");
    this->resize(scalpmapPixmap->width()/scalling,scalpmapPixmap->height()/scalling);

    setHardware("emotiv");

    electrodes["emotiv"]["P7"].append(291);
    electrodes["emotiv"]["P7"].append(316);
    electrodes["emotiv"]["P7"].append(0);

    electrodes["emotiv"]["P8"].append(534);
    electrodes["emotiv"]["P8"].append(316);
    electrodes["emotiv"]["P8"].append(0);


    electrodes["emotiv"]["FC6"].append(540);
    electrodes["emotiv"]["FC6"].append(202);
    electrodes["emotiv"]["FC6"].append(0);

    electrodes["emotiv"]["FC5"].append(291);
    electrodes["emotiv"]["FC5"].append(202);
    electrodes["emotiv"]["FC5"].append(0);


    electrodes["emotiv"]["T7"].append(240);
    electrodes["emotiv"]["T7"].append(246);
    electrodes["emotiv"]["T7"].append(0);

    electrodes["emotiv"]["T8"].append(590);
    electrodes["emotiv"]["T8"].append(246);
    electrodes["emotiv"]["T8"].append(0);



    electrodes["emotiv"]["O1"].append(373);
    electrodes["emotiv"]["O1"].append(343);
    electrodes["emotiv"]["O1"].append(0);

    electrodes["emotiv"]["O2"].append(450);
    electrodes["emotiv"]["O2"].append(343);
    electrodes["emotiv"]["O2"].append(0);


    electrodes["emotiv"]["F3"].append(348);
    electrodes["emotiv"]["F3"].append(171);
    electrodes["emotiv"]["F3"].append(0);

    electrodes["emotiv"]["F4"].append(484);
    electrodes["emotiv"]["F4"].append(171);
    electrodes["emotiv"]["F4"].append(0);



    electrodes["emotiv"]["F7"].append(269);
    electrodes["emotiv"]["F7"].append(154);
    electrodes["emotiv"]["F7"].append(0);

    electrodes["emotiv"]["F8"].append(562);
    electrodes["emotiv"]["F8"].append(154);
    electrodes["emotiv"]["F8"].append(0);


    electrodes["emotiv"]["AF3"].append(366);
    electrodes["emotiv"]["AF3"].append(125);
    electrodes["emotiv"]["AF3"].append(0);


    electrodes["emotiv"]["AF4"].append(464);
    electrodes["emotiv"]["AF4"].append(125);
    electrodes["emotiv"]["AF4"].append(0);


    electrodes["emocap"]["TP10"].append(580);
    electrodes["emocap"]["TP10"].append(275);
    electrodes["emocap"]["TP10"].append(0);
    electrodes["emocap"]["TP9"].append(250);
    electrodes["emocap"]["TP9"].append(275);
    electrodes["emocap"]["TP9"].append(0);

    electrodes["emocap"]["FZ"].append(415);
    electrodes["emocap"]["FZ"].append(177);
    electrodes["emocap"]["FZ"].append(0);

    electrodes["emocap"]["P3"].append(340);
    electrodes["emocap"]["P3"].append(316);
    electrodes["emocap"]["P3"].append(0);
    electrodes["emocap"]["P4"].append(483);
    electrodes["emocap"]["P4"].append(316);
    electrodes["emocap"]["P4"].append(0);

    electrodes["emocap"]["CZ"].append(415);
    electrodes["emocap"]["CZ"].append(246);
    electrodes["emocap"]["CZ"].append(0);

    electrodes["emocap"]["C4"].append(495);
    electrodes["emocap"]["C4"].append(246);
    electrodes["emocap"]["C4"].append(0);
    electrodes["emocap"]["C3"].append(335);
    electrodes["emocap"]["C3"].append(246);
    electrodes["emocap"]["C3"].append(0);

    electrodes["emocap"]["PZ"].append(415);
    electrodes["emocap"]["PZ"].append(306);
    electrodes["emocap"]["PZ"].append(0);

    electrodes["emocap"]["FPZ"].append(415);
    electrodes["emocap"]["FPZ"].append(100);
    electrodes["emocap"]["FPZ"].append(0);

    electrodes["emocap"]["O1"].append(373);
    electrodes["emocap"]["O1"].append(343);
    electrodes["emocap"]["O1"].append(0);

    electrodes["emocap"]["O2"].append(450);
    electrodes["emocap"]["O2"].append(343);
    electrodes["emocap"]["O2"].append(0);

    electrodes["emocap"]["F3"].append(348);
    electrodes["emocap"]["F3"].append(171);
    electrodes["emocap"]["F3"].append(0);

    electrodes["emocap"]["F4"].append(484);
    electrodes["emocap"]["F4"].append(171);
    electrodes["emocap"]["F4"].append(0);


}


void ScalpMap::paintEvent(QPaintEvent *event, QPainter* painter)
{


    painter->drawPixmap(this->x(),this->y(),this->width(),this->height(),*scalpmapPixmap);

    foreach (QVector<double> electrode, electrodes[hardware])
    {
	painter->setPen(QPen(getElectrodeColor(electrode.at(2))));
	painter->setBrush(QBrush(getElectrodeColor(electrode.at(2))));
	painter->drawRoundedRect(this->x() + electrode.at(0)/scalling - 10, this->y() + electrode.at(1)/scalling, 20,20,5,5);
    }

}

void ScalpMap::setHardware(QString hardware_)
{
    hardware = hardware_;
}

void ScalpMap::setCq(QString channel, double cq)
{

    if (electrodes[hardware].contains(channel))
	electrodes[hardware][channel][2] = cq;

    update();
}

QColor ScalpMap::getElectrodeColor(double value)
{

    if (value < 81)
	return QColor("black");
    if (value < 221)
	return QColor("red");
    if (value < 314)
	return QColor("orange");
    if (value < 407)
	return QColor("yellow");
    if (value >= 407)
	return QColor("lime");

    return QColor("blue");


}
