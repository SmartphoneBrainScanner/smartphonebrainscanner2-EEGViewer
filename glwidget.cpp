#include "glwidget.h"

#if defined(Q_OS_MAC)
#include <OpenGL.h>
#endif

GLWidget::GLWidget(MyCallback *myCallback_, QWidget *parent) :
    QGLWidget(parent), timer(new QBasicTimer), myCallback(myCallback_)
{
    painter = new QPainter(this);
    setAutoBufferSwap(false);

    values = 256;
    currentPosition = 0;
    valuesSpectro = Sbs2Common::samplingRate()/2;

    pathGenerated = 0;
    pathSpectroGenerated = 0;

    gyroX = gyroY = 0;

    startY = -1;
    scale = 20;
    lastD = 0;
    startX = -1;
    currentX = 0;

    timeSeries = 1;
    filterSetting = 0;
    scalpmapVisible = 0;

    paused = 0;

    filterText = "filter off";
    timeSeriesText = "time series";
    playText = "playing";
    scalpmapText = "values";

    scalpmap = new ScalpMap(1,this);
    scalpmap->move(240,150);

    QObject::connect(myCallback,SIGNAL(hardwareChanged(QString)), scalpmap, SLOT(setHardware(QString)));
    QObject::connect(myCallback, SIGNAL(cqValue(QString,double)),scalpmap,SLOT(setCq(QString,double)));
}

void GLWidget::gyroSlot(int gyroX_, int gyroY_)
{
    gyroX = gyroX_;
    gyroY = gyroY_;

    gyroPosX += (1660-gyroX)/10.0;
    if (gyroPosX > this->w) gyroPosX = this->w;
    if (gyroPosX < 0) gyroPosX = 0;
    if (gyroPosX < this->w/2) gyroPosX += this->w/500.0;
    if (gyroPosX > this->w/2) gyroPosX -= this->w/500.0;

    gyroPosY -= (1660-gyroY)/10.0;
    if (gyroPosY > this->h) gyroPosY = this->h;
    if (gyroPosY < 0) gyroPosY = 0;
    if (gyroPosY < this->h/2) gyroPosY += this->h/500.0;
    if (gyroPosY > this->h/2) gyroPosY -= this->h/500.0;

}

void GLWidget::resizeGL(int w, int h)
{
    this->w = w;
    this->h = h;
    gyroPosX = this->w/2.0;
    gyroPosY = this->h/2.0;
    aspectRatio = (qreal) w / (qreal) h;
    generatePaths();
    generatePathsSpectro();
    generateUI();
}

void GLWidget::generatePaths()
{
    if (this->w != 1280)
	return;
    if (pathGenerated)
	return;

    pathGenerated = 1;
    valueToValueDistance = floor(this->w/this->values);

    for (int channel = 0; channel < Sbs2Common::channelsNo(); ++channel)
    {
	channels.append(new QPainterPath());
	rawValues.append(QList<int>());
	channels.last()->moveTo(0,(channel+1)*50);
	for (int i = 0; i < values-1; ++i)
	{
	    channels.last()->lineTo(channels.last()->currentPosition().x() + this->valueToValueDistance,channels.last()->currentPosition().y());
	    rawValues[channel].append(0);
	}
	rawValues[channel].append(0);
    }
    generateGrid();
}

void GLWidget::generatePathsSpectro()
{
    if (this->w != 1280)
	return;
    if (pathSpectroGenerated)
	return;

    pathSpectroGenerated = 1;
    valueToValueSpectroDistance = floor(this->w/valuesSpectro);

    for (int channel = 0; channel < Sbs2Common::channelsNo(); ++channel)
    {
	channelsSpectro.append(new QPainterPath());
	rawValuesSpectro.append(QList<double>());
	channelsSpectro.last()->moveTo(0,(channel+1)*50);
	for (int i = 0; i < valuesSpectro-1; ++i)
	{
	    channelsSpectro.last()->lineTo(channelsSpectro.last()->currentPosition().x() + this->valueToValueSpectroDistance,channelsSpectro.last()->currentPosition().y());

	    rawValuesSpectro[channel].append(0);
	}
	rawValuesSpectro[channel].append(0);
    }
    generateGridSpectro();
}

void GLWidget::generateGrid()
{
    for (int i = 0; i < values/32 - 1; ++i)
    {
	gridLines.append(new QLine(channels.at(0)->elementAt((i+1)*32).x,0,channels.at(0)->elementAt((i+1)*32).x,this->h));
    }
}

void GLWidget::generateGridSpectro()
{
    for (int i= 0; i < valuesSpectro; ++i)
    {
	if (i%10==0 && i > 0)
	{
	    gridLinesSpectro.append(new QLine(channelsSpectro.at(0)->elementAt(i).x,0,channelsSpectro.at(0)->elementAt(i).x,this->h));
	}
    }
}

void GLWidget::generateUI()
{
    toggleFilterRect = QRect(20,this->h - 50, 200, 50);
    toggleTimeSeriesRect = QRect(toggleFilterRect.x() + toggleFilterRect.width() + 20, toggleFilterRect.y(), toggleFilterRect.width(), toggleFilterRect.height());
    togglePlayRect = QRect(toggleTimeSeriesRect.x() + toggleTimeSeriesRect.width() + 20, toggleFilterRect.y(), toggleFilterRect.width(), toggleFilterRect.height());
    toggleHardwareRect = QRect(togglePlayRect.x() + togglePlayRect.width() + 20, toggleFilterRect.y(), toggleFilterRect.width(), toggleFilterRect.height());
    toggleScalpmapRect = QRect(toggleHardwareRect.x() + toggleHardwareRect.width() + 20, toggleFilterRect.y(), toggleFilterRect.width(), toggleFilterRect.height());
    quitRect = QRect(this->w - toggleFilterRect.width() + 60, toggleFilterRect.y(), toggleFilterRect.width() - 60, toggleFilterRect.height());
}

void GLWidget::update(int index)
{
    if (paused)
	return;

    QVector<QList<int> > vs = myCallback->values[index];

    currentPosition = (currentPosition+vs.at(0).size())%values;
    for (int channel = 0; channel<vs.size(); ++channel)
    {
	int mean = 0;
	for (int s = 0; s < rawValues.at(channel).size(); ++s)
	    mean += rawValues.at(channel).at(s);

	mean /= rawValues.at(channel).size();

	int u = vs.at(0).size()-1;
	for (int p = (currentPosition - 1); p >= (currentPosition - vs.at(0).size()); --p)
	{
	    int pp = p;
	    if (pp < 0) pp = this->values + pp;
	    double v = (mean - vs.at(channel).at(u))/scale + (channel+1)*50;
	    channels.at(channel)->setElementPositionAt(pp,channels.at(channel)->elementAt(pp).x,v);
	    rawValues[channel][pp] = vs.at(channel).at(u);
	    --u;
	}
    }
}

void GLWidget::updateSpectro()
{
    if (paused)
	return;

    QVector<QVector<double> > vs = myCallback->valuesSpectro;

    for (int channel = 0; channel < vs.size(); ++channel)
    {
	for (int bin = 0; bin < valuesSpectro; ++bin)
	{
	    rawValuesSpectro[channel][bin] = 20* log10(vs.at(channel).at(bin));
	}

	for (int p = 0; p < valuesSpectro; ++p)
	{
	    double v = (- rawValuesSpectro.at(channel).at(p) * 10.0)/scale + (channel+2)*50;
	    channelsSpectro.at(channel)->setElementPositionAt(p,channelsSpectro.at(channel)->elementAt(p).x,v);
	}
    }
}

void GLWidget::initializeGL()
{
#if defined(Q_OS_MAC)
    const GLint swapInterval = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &swapInterval);
#endif
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    timer->start(1/50 * 1000.0, this);
}

void GLWidget::paintGL()
{

    painter->begin(this);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if (timeSeries)
    {
	painter->setPen(QPen(QBrush(QColor(0,0,0,120)),1));
	foreach (QLine* gridLine, gridLines)
	{
	    painter->drawLine(*gridLine);
	}
    }
    else
    {
	painter->setPen(QPen(QBrush(QColor(0,0,0,120)),1));
	foreach (QLine* gridLine, gridLinesSpectro)
	{
	    painter->drawLine(*gridLine);
	}

    }

    if (timeSeries)
    {
	painter->setPen(QPen(QBrush(QColor("red")),2));
	painter->drawLine(channels.at(0)->elementAt(currentPosition).x,0,channels.at(0)->elementAt(currentPosition).x,this->h);
    }

    if (!scalpmapVisible)
    {
	painter->fillRect(toggleFilterRect, QBrush(QColor("black")));
	painter->fillRect(toggleTimeSeriesRect, QBrush(QColor("black")));
	painter->fillRect(togglePlayRect,QBrush(QColor("black")));
	painter->fillRect(quitRect, QBrush(QColor("black")));
	painter->fillRect(toggleHardwareRect, QBrush(QColor("black")));
	painter->fillRect(toggleScalpmapRect, QBrush(QColor("black")));


	painter->setFont(QFont("helvetica",8));
	painter->setPen(QColor("white"));
	painter->drawText(toggleFilterRect, Qt::AlignCenter, filterText);
	painter->drawText(toggleTimeSeriesRect, Qt::AlignCenter, timeSeriesText);
	painter->drawText(togglePlayRect, Qt::AlignCenter, playText);
	painter->drawText(quitRect, Qt::AlignCenter, "quit");
	painter->drawText(toggleHardwareRect, Qt::AlignCenter, Sbs2Common::getCurrentHardware());
	painter->drawText(toggleScalpmapRect, Qt::AlignCenter, scalpmapText);
    }


    for (int j = 0; j < channels.size(); ++j)
    {
	if (j == 0)
	    painter->setPen(QPen(QBrush(QColor("blue")),2));
	if (j == 1)
	    painter->setPen(QPen(QBrush(QColor("green")),2));
	if (j == 2)
	    painter->setPen(QPen(QBrush(QColor("red")),2));
	if (j == 3)
	    painter->setPen(QPen(QBrush(QColor("black")),2));
	if (j == 4)
	    painter->setPen(QPen(QBrush(QColor("purple")),2));
	if (j == 5)
	    painter->setPen(QPen(QBrush(QColor("orange")),2));
	if (j == 6)
	    painter->setPen(QPen(QBrush(QColor("indigo")),2));
	if (j == 7)
	    painter->setPen(QPen(QBrush(QColor("olive")),2));
	if (j == 8)
	    painter->setPen(QPen(QBrush(QColor("salmon")),2));
	if (j == 9)
	    painter->setPen(QPen(QBrush(QColor("magenta")),2));
	if (j == 10)
	    painter->setPen(QPen(QBrush(QColor("lime")),2));
	if (j == 11)
	    painter->setPen(QPen(QBrush(QColor("darkred")),2));
	if (j == 12)
	    painter->setPen(QPen(QBrush(QColor("navy")),2));
	if (j == 13)
	    painter->setPen(QPen(QBrush(QColor("brown")),2));

	if (timeSeries)
	    painter->drawPath(*(channels.at(j)));
	else
	    painter->drawPath(*(channelsSpectro.at(j)));

    }



    painter->setFont(QFont("helvetica",8));
    painter->setPen(QColor("black"));
    for (int i=0; i<Sbs2Common::channelsNo();++i)
    {
	painter->drawText(0,(i+1)*50,Sbs2Common::getChannelNames()->at(i));
    }





    painter->drawEllipse(gyroPosX-5,gyroPosY-5,10,10);

    if (startY != -1)
    {
	painter->setFont(QFont("helvetica",80));
	painter->drawText(this->w/2 - 80,this->h/2 - 20,QString::number(scale));
    }

    if (scalpmapVisible)
    {
	painter->fillRect(this->rect(),QBrush(QColor("white")));
	scalpmap->paintEvent(0, painter);
	painter->fillRect(quitRect, QBrush(QColor("black")));
	painter->fillRect(toggleHardwareRect, QBrush(QColor("black")));
	painter->fillRect(toggleScalpmapRect, QBrush(QColor("black")));
	painter->setFont(QFont("helvetica",8));
	painter->setPen(QColor("white"));
	painter->drawText(quitRect, Qt::AlignCenter, "quit");
	painter->drawText(toggleHardwareRect, Qt::AlignCenter, Sbs2Common::getCurrentHardware());
	painter->drawText(toggleScalpmapRect, Qt::AlignCenter, scalpmapText);
    }


    painter->end();
    swapBuffers();

}

void GLWidget::timerEvent(QTimerEvent *)
{
    updateGL();
}

void GLWidget::toggleFilter()
{
    filterSetting = !filterSetting;

    if (filterSetting)
    {
	emit turnFilterOn(8,30,32);
	filterText = "8-30Hz";
    }
    else
    {
	emit turnFilterOff();
	filterText = "filter off";
    }

}

void GLWidget::toggleTimeSeries()
{

    timeSeries = !timeSeries;

    if (timeSeries)
    {
	emit turnSpectrogramOff();
	timeSeriesText = "time series";
    }
    else
    {
	emit turnSpectrogramOn(128,128,32);
	timeSeriesText = "frequency";
    }
}

void GLWidget::togglePlay()
{
    paused = !paused;
    if (paused)
    {
	playText = "paused";
    }
    else
    {
	playText = "playing";
    }
}

void GLWidget::toggleHardware()
{
    if (Sbs2Common::getCurrentHardware() == "emocap")
	myCallback->setHardware("emotiv");
    else
	myCallback->setHardware("emocap");
}

void GLWidget::toggleScalpmap()
{
    scalpmapVisible = !scalpmapVisible;

    if (scalpmapVisible)
    {
	scalpmapText = "quality";
    }
    else
    {
	scalpmapText = "values";
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    if (event->button() != Qt::LeftButton)
	return;

    if (toggleFilterRect.contains(event->pos()))
    {
	toggleFilter();
	return;
    }

    if (toggleTimeSeriesRect.contains(event->pos()))
    {
	toggleTimeSeries();
	return;
    }

    if (togglePlayRect.contains(event->pos()))
    {
	togglePlay();
	return;
    }

    if (toggleHardwareRect.contains(event->pos()))
    {
	toggleHardware();
	return;
    }
    if (toggleScalpmapRect.contains(event->pos()))
    {
	toggleScalpmap();
	return;
    }

    if (quitRect.contains(event->pos()))
    {
	QApplication::quit();
    }

    startX = event->pos().x();
    currentX = startX;
    startY = event->pos().y();
    lastD = 0;

}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (startY == -1)
	return;

    if (startX == -1)
	return;

    int v = (int)(event->pos().y() - startY)/50;

    if (v != lastD)
    {

	scale += v;

	if (scale < 1.0) scale = 1.0;
	if (scale > 100.0) scale = 100.0;

	startY = event->pos().y();

    }

    lastD = v;

    currentX = event->pos().x();

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    startY = -1;
    lastD = 0;
    startX = -1;
}

void GLWidget::kill()
{
    system("su -c 'killall -9 org.kde.necessitas.example.EegViewer'");
}


